#pragma once

#include <lrcpp/Components.h>

#include <stdint.h>

class Vfs : public lrcpp::VirtualFileSystem {
public:
    Vfs();

    bool init(lrcpp::Logger* logger);
    void destroy();

    // lrcpp::VirtualFileSystem
    virtual bool getVfsInterface(retro_vfs_interface_info* info) override;

    virtual char const* getPath(retro_vfs_file_handle* stream) override;
    virtual retro_vfs_file_handle* open(char const* path, unsigned mode, unsigned hints) override;
    virtual int close(retro_vfs_file_handle* stream) override;
    virtual int64_t size(retro_vfs_file_handle* stream) override;
    virtual int64_t truncate(retro_vfs_file_handle* stream, int64_t length) override;
    virtual int64_t tell(retro_vfs_file_handle* stream) override;
    virtual int64_t seek(retro_vfs_file_handle* stream, int64_t offset, int seekPosition) override;
    virtual int64_t read(retro_vfs_file_handle* stream, void* s, uint64_t len) override;
    virtual int64_t write(retro_vfs_file_handle* stream, void const* s, uint64_t len) override;
    virtual int flush(retro_vfs_file_handle* stream) override;
    virtual int remove(char const* path) override;
    virtual int rename(char const* oldPath, char const* newPath) override;
    virtual int stat(char const* path, int32_t* size) override;
    virtual int mkDir(char const* dir) override;
    virtual retro_vfs_dir_handle* openDir(char const* dir, bool includeHidden) override;
    virtual bool readDir(retro_vfs_dir_handle* dirstream) override;
    virtual char const* direntGetName(retro_vfs_dir_handle* dirstream) override;
    virtual bool direntIsDir(retro_vfs_dir_handle* dirstream) override;
    virtual int closeDir(retro_vfs_dir_handle* dirstream) override;

protected:
    lrcpp::Logger* _logger;
};
