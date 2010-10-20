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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>

#include "certificate.h"
#include "sign.h"

/* 
 * Static functions definition
 */
static xmlNodePtr nofl_sign_create_template(nofl_certificate *cert);
static int nofl_sign_add_signature(nofl_certificate *cert);


int 
nofl_sign_certificate(nofl_certificate *cert) {

  xmlDocPtr doc;

  doc = cert->document;

  nofl_sign_create_template(cert);
  nofl_sign_add_signature(cert);
  
  return 0;
}

static xmlNodePtr
nofl_sign_create_template(nofl_certificate *cert) {

  xmlNodePtr template_signature;
  xmlNodePtr reference;
  xmlNodePtr keyinfo;

  template_signature = xmlSecTmplSignatureCreate(cert->document,
						 cert->c14n_method,
						 cert->sign_method,
						 NULL);   //"nofl-cert-signature");
  xmlAddChild(xmlDocGetRootElement(cert->document), template_signature);
  reference = xmlSecTmplSignatureAddReference(template_signature, 
					      xmlSecTransformSha1Id,
					      NULL, 
					      NULL, 
					      NULL);
  xmlSecTmplReferenceAddTransform(reference, xmlSecTransformEnvelopedId);
  keyinfo = xmlSecTmplSignatureEnsureKeyInfo(template_signature, NULL);
  xmlSecTmplKeyInfoAddKeyName(keyinfo, NULL);

  cert->signature_node = template_signature;

  return template_signature;
}

static int 
nofl_sign_add_signature(nofl_certificate *cert) {

    xmlSecDSigCtxPtr dsig_context = NULL;  

    dsig_context = xmlSecDSigCtxCreate(NULL);
    dsig_context->signKey = xmlSecCryptoAppKeyLoad(cert->key_path, 
						   xmlSecKeyDataFormatPem, 
						   NULL, 
						   xmlSecCryptoAppGetDefaultPwdCallback(),
						   NULL); //cert->key_path);

    xmlSecKeySetName(dsig_context->signKey, cert->key_path);

    return xmlSecDSigCtxSign(dsig_context, cert->signature_node);
}
