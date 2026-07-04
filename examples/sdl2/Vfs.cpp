#include "Vfs.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

#include <string>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#define FSEEK64 _fseeki64
#define FTELL64 _ftelli64
#else
#include <unistd.h>
#define FSEEK64 fseeko
#define FTELL64 ftello
#endif

struct retro_vfs_file_handle {
    FILE* fp;
    std::string path;
};

struct retro_vfs_dir_handle {
    DIR* dir;
    struct dirent* entry;
    std::string path;
    bool includeHidden;
};

Vfs::Vfs() {
    _logger = nullptr;
}

bool Vfs::init(lrcpp::Logger* logger) {
    _logger = logger;
    return true;
}

void Vfs::destroy() {}

bool Vfs::getVfsInterface(retro_vfs_interface_info* info) {
    if (info->required_interface_version > 3) {
        _logger->warn("Core requires VFS version %u, only up to 3 is supported\n", info->required_interface_version);
        return false;
    }

    info->required_interface_version = 3;
    _logger->info("Providing VFS interface version 3\n");
    return true;
}

char const* Vfs::getPath(retro_vfs_file_handle* stream) {
    return stream->path.c_str();
}

retro_vfs_file_handle* Vfs::open(char const* path, unsigned mode, unsigned hints) {
    (void)hints;

    char const* fmode;
    unsigned const access = mode & RETRO_VFS_FILE_ACCESS_READ_WRITE;
    bool const keep = (mode & RETRO_VFS_FILE_ACCESS_UPDATE_EXISTING) != 0;

    if (access == RETRO_VFS_FILE_ACCESS_READ) {
        fmode = "rb";
    }
    else if (access == RETRO_VFS_FILE_ACCESS_WRITE) {
        fmode = keep ? "r+b" : "wb";
    }
    else {
        fmode = keep ? "r+b" : "w+b";
    }

    FILE* const fp = fopen(path, fmode);

    if (fp == nullptr) {
        _logger->error("Could not open \"%s\": %s\n", path, strerror(errno));
        return nullptr;
    }

    return new retro_vfs_file_handle{fp, path};
}

int Vfs::close(retro_vfs_file_handle* stream) {
    int const rc = fclose(stream->fp);
    delete stream;
    return rc == 0 ? 0 : -1;
}

int64_t Vfs::size(retro_vfs_file_handle* stream) {
    int64_t const current = FTELL64(stream->fp);

    if (FSEEK64(stream->fp, 0, SEEK_END) != 0) {
        return -1;
    }

    int64_t const length = FTELL64(stream->fp);
    FSEEK64(stream->fp, current, SEEK_SET);
    return length;
}

int64_t Vfs::truncate(retro_vfs_file_handle* stream, int64_t length) {
    fflush(stream->fp);

#ifdef _WIN32
    return _chsize_s(_fileno(stream->fp), length) == 0 ? 0 : -1;
#else
    return ftruncate(fileno(stream->fp), length) == 0 ? 0 : -1;
#endif
}

int64_t Vfs::tell(retro_vfs_file_handle* stream) {
    return FTELL64(stream->fp);
}

int64_t Vfs::seek(retro_vfs_file_handle* stream, int64_t offset, int seekPosition) {
    int whence;

    switch (seekPosition) {
        case RETRO_VFS_SEEK_POSITION_START:   whence = SEEK_SET; break;
        case RETRO_VFS_SEEK_POSITION_CURRENT: whence = SEEK_CUR; break;
        case RETRO_VFS_SEEK_POSITION_END:     whence = SEEK_END; break;
        default: return -1;
    }

    if (FSEEK64(stream->fp, offset, whence) != 0) {
        return -1;
    }

    return FTELL64(stream->fp);
}

int64_t Vfs::read(retro_vfs_file_handle* stream, void* s, uint64_t len) {
    size_t const numRead = fread(s, 1, len, stream->fp);

    if (numRead < len && ferror(stream->fp) != 0) {
        return -1;
    }

    return static_cast<int64_t>(numRead);
}

int64_t Vfs::write(retro_vfs_file_handle* stream, void const* s, uint64_t len) {
    size_t const numWritten = fwrite(s, 1, len, stream->fp);

    if (numWritten < len && ferror(stream->fp) != 0) {
        return -1;
    }

    return static_cast<int64_t>(numWritten);
}

int Vfs::flush(retro_vfs_file_handle* stream) {
    return fflush(stream->fp) == 0 ? 0 : -1;
}

int Vfs::remove(char const* path) {
    return ::remove(path) == 0 ? 0 : -1;
}

int Vfs::rename(char const* oldPath, char const* newPath) {
    return ::rename(oldPath, newPath) == 0 ? 0 : -1;
}

int Vfs::stat(char const* path, int32_t* size) {
    struct stat statbuf;

    if (::stat(path, &statbuf) != 0) {
        return 0;
    }

    int flags = RETRO_VFS_STAT_IS_VALID;

    if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
        flags |= RETRO_VFS_STAT_IS_DIRECTORY;
    }
    else if ((statbuf.st_mode & S_IFMT) == S_IFCHR) {
        flags |= RETRO_VFS_STAT_IS_CHARACTER_SPECIAL;
    }

    if (size != nullptr) {
        *size = static_cast<int32_t>(statbuf.st_size);
    }

    return flags;
}

int Vfs::mkDir(char const* dir) {
#ifdef _WIN32
    int const rc = _mkdir(dir);
#else
    int const rc = ::mkdir(dir, 0755);
#endif

    if (rc == 0) {
        return 0;
    }

    return errno == EEXIST ? -2 : -1;
}

retro_vfs_dir_handle* Vfs::openDir(char const* dir, bool includeHidden) {
    DIR* const handle = opendir(dir);

    if (handle == nullptr) {
        _logger->error("Could not open directory \"%s\": %s\n", dir, strerror(errno));
        return nullptr;
    }

    return new retro_vfs_dir_handle{handle, nullptr, dir, includeHidden};
}

bool Vfs::readDir(retro_vfs_dir_handle* dirstream) {
    for (;;) {
        struct dirent* const entry = readdir(dirstream->dir);

        if (entry == nullptr) {
            dirstream->entry = nullptr;
            return false;
        }

        if (!dirstream->includeHidden && entry->d_name[0] == '.') {
            continue;
        }

        dirstream->entry = entry;
        return true;
    }
}

char const* Vfs::direntGetName(retro_vfs_dir_handle* dirstream) {
    return dirstream->entry->d_name;
}

bool Vfs::direntIsDir(retro_vfs_dir_handle* dirstream) {
    std::string const path = dirstream->path + "/" + dirstream->entry->d_name;

    struct stat statbuf;

    if (::stat(path.c_str(), &statbuf) != 0) {
        return false;
    }

    return (statbuf.st_mode & S_IFMT) == S_IFDIR;
}

int Vfs::closeDir(retro_vfs_dir_handle* dirstream) {
    int const rc = closedir(dirstream->dir);
    delete dirstream;
    return rc == 0 ? 0 : -1;
}
