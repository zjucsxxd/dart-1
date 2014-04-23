/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codename Pegasus Operating System - Defines the interface for and     *
 *                                   - structures relating to the virtual*
 *                                   - file system.                      *
 * Copyright (C) 2011  Constantine M. Apostolou                          *
 * Author: Constantine M. Apostolou - conmarap@osarena.net               *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "fs.h"

fs_node_t *fs_root = 0; // The root of the filesystem.

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Read the file system at the selected node   *
 * * * * * * * * * * * * * * * * * * * * * * * * */
u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
    // Has the node got a read callback?
    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Write the file system at the selected node  *
 * * * * * * * * * * * * * * * * * * * * * * * * */
u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
    // Has the node got a write callback?
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *   Open the file system at the selected node   *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void open_fs(fs_node_t *node, u8int read, u8int write)
{
    // Has the node got an open callback?
    if (node->open != 0)
        return node->open(node);
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *            Open the file system node          *
 * * * * * * * * * * * * * * * * * * * * * * * * */
void close_fs(fs_node_t *node)
{
    // Has the node got a close callback?
    if (node->close != 0)
        return node->close(node);
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *       Read the file system directory          *
 * * * * * * * * * * * * * * * * * * * * * * * * */
struct dirent *readdir_fs(fs_node_t *node, u32int index)
{
    // Is the node a directory, and does it have a callback?
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->readdir != 0 )
        return node->readdir(node, index);
    else
        return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *       Find the file system directory          *
 * * * * * * * * * * * * * * * * * * * * * * * * */
fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
    // Is the node a directory, and does it have a callback?
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->finddir != 0 )
        return node->finddir(node, name);
    else
        return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         End of file fs.c                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
