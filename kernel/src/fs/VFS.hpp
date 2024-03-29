/*
Copyright (©) 2023-2024  Frosty515

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _VFS_HPP
#define _VFS_HPP

#include <stdint.h>
#include <stddef.h>

#include "DirectoryStream.hpp"
#include "FileSystem.hpp"
#include "Inode.hpp"

#include <Data-structures/LinkedList.hpp>


constexpr uint8_t VFS_READ = 1;
constexpr uint8_t VFS_WRITE = 2;


struct VFS_MountPoint {
    FileSystemType type;
    FileSystem* fs;
    Inode* RootInode;
    VFS_MountPoint* parent;
};

struct VFS_WorkingDirectory {
    VFS_MountPoint* mountpoint;
    Inode* inode;
};

class FileStream; // defined in FileStream.hpp

class VFS final : public FileSystem {
public:
    VFS();
    VFS(FileSystemType root_type);
    ~VFS();

    bool MountRoot(FileSystemType type);
    bool Mount(FilePrivilegeLevel current_privilege, const char* path, FileSystemType type);
    bool Unmount(FilePrivilegeLevel current_privilege, const char* path, VFS_WorkingDirectory* working_directory = nullptr);

    bool CreateFile(FilePrivilegeLevel current_privilege, const char* parent, const char* name, size_t size = 0, bool inherit_permissions = true, FilePrivilegeLevel privilege = {0, 0, 00644}) override;
    bool CreateFolder(FilePrivilegeLevel current_privilege, const char* parent, const char* name, bool inherit_permissions = true, FilePrivilegeLevel privilege = {0, 0, 00644}) override;
    bool CreateSymLink(FilePrivilegeLevel current_privilege, const char* parent, const char* name, const char* target, bool inherit_permissions = true, FilePrivilegeLevel privilege = {0, 0, 00644}) override;

    bool DeleteInode(FilePrivilegeLevel current_privilege, const char* path, bool recursive = false, bool delete_name = false) override; // delete_name is not used by the VFS as it **always** deletes the name

    FileStream* OpenStream(FilePrivilegeLevel current_privilege, const char* path, uint8_t modes, VFS_WorkingDirectory* working_directory = nullptr);
    bool CloseStream(FileStream* stream);

    DirectoryStream* OpenDirectoryStream(FilePrivilegeLevel current_privilege, const char* path, uint8_t modes, VFS_WorkingDirectory* working_directory = nullptr);
    bool CloseDirectoryStream(DirectoryStream* stream);

    bool IsValidPath(const char* path, VFS_WorkingDirectory* working_directory = nullptr) const;

    Inode* GetInode(const char* path, VFS_WorkingDirectory* working_directory = nullptr, FileSystem** fs = nullptr, VFS_MountPoint** mountpoint = nullptr) const;

    VFS_MountPoint* GetMountPoint(FileSystem* fs) const;

    VFS_WorkingDirectory* GetRootWorkingDirectory() const;

private:
    VFS_MountPoint* GetMountPoint(const char* path, VFS_WorkingDirectory* working_directory = nullptr, Inode** inode = nullptr) const;

    // Get the mountpoint for a child of the given path.
    VFS_MountPoint* GetChildMountPoint(const char* path, VFS_WorkingDirectory* working_directory = nullptr, Inode** inode = nullptr) const;

    bool isMountpoint(const char* path, size_t len, VFS_WorkingDirectory* working_directory = nullptr); // When false is returned, the caller **MUST** check for any errors.

private:
    VFS_MountPoint* m_root;

    LinkedList::SimpleLinkedList<VFS_MountPoint> m_mountPoints;
    LinkedList::SimpleLinkedList<FileStream> m_streams;
    LinkedList::SimpleLinkedList<DirectoryStream> m_directoryStreams;
};

extern VFS* g_VFS;

#endif /* _VFS_HPP */