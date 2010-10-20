/*
 * This file is part of nofl
 * Copyright (C) 2009 Cleber Rodrigues <cleber.gnu@gmail.com>
 * All rights reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 * 
 * Author(s): Cleber Rodrigues <cleber.gnu@gmail.com>
 */

#ifndef _INIT_H_
#define _INIT_H_

#define NOFL_INIT_SUCCESS      0x00000000
#define NOFL_INIT_ERROR_LIBXML 0x00000001
#define NOFL_INIT_ERROR_XMLSEC 0x00000002

#define NOFL_SHUTDOWN_SUCCESS  0x00000000

#define NOFL_NOT_INITIALIZED   0x00000000
#define NOFL_INITIALIZED       0x00000001

int nofl_initialize(void);
int nofl_shudown(void);
int nofl_initialized(void);

#endif /* _INIT_H_ */
