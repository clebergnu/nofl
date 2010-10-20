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

#ifndef _CERTIFICATE_H_
#define _CERTIFICATE_H_

#include <libxml/parser.h>
#include <libxml/xmlwriter.h>

#include <xmlsec/transforms.h>

struct nofl_certificate_t {

  char *name;

  char *key_path;
  char *output_path;

  xmlDocPtr document;
  xmlNodePtr root_node;
  xmlNodePtr signature_node;

  xmlSecTransformId c14n_method;
  xmlSecTransformId sign_method;

};

typedef struct nofl_certificate_t nofl_certificate;

nofl_certificate *nofl_certificate_create(const char *name);
void nofl_certificate_destroy(nofl_certificate *cert);
void nofl_certificate_dump(nofl_certificate *cert);

char *nofl_certificate_get_name(nofl_certificate *cert);

void nofl_certificate_set_version(nofl_certificate *cert, char *version);
char *nofl_certificate_get_version(nofl_certificate *cert);

void nofl_certificate_set_customer_name(nofl_certificate *cert, char *name);
char *nofl_certificate_get_customer_name(nofl_certificate *cert);

void nofl_certificate_set_customer_id(nofl_certificate *cert, char *id);
char *nofl_certificate_get_customer_id(nofl_certificate *cert);

void nofl_certificate_set_product_name(nofl_certificate *cert, char *name);
char *nofl_certificate_get_product_name(nofl_certificate *cert);

void nofl_certificate_set_product_version(nofl_certificate *cert, char *version);
char *nofl_certificate_get_product_version(nofl_certificate *cert);

void nofl_certificate_set_product_min_version(nofl_certificate *cert, char *version);
char *nofl_certificate_get_product_min_version(nofl_certificate *cert);

void nofl_certificate_set_product_max_version(nofl_certificate *cert, char *version);
char *nofl_certificate_get_product_max_version(nofl_certificate *cert);

void nofl_certificate_set_initial_date(nofl_certificate *cert, char *date);
char *nofl_certificate_get_initial_date(nofl_certificate *cert);

void nofl_certificate_set_expire_date(nofl_certificate *cert, char *date);
char *nofl_certificate_get_expire_date(nofl_certificate *cert);

void nofl_certificate_set_expire_grace(nofl_certificate *cert, int grace);
char *nofl_certificate_get_expire_grace(nofl_certificate *cert);

nofl_certificate *nofl_certificate_load(const char *path);
void nofl_certificate_save(nofl_certificate *cert);

#endif	/* _CERTIFICATE_H_ */
