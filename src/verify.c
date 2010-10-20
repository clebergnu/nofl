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

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/xmldsig.h>

#include "certificate.h"

int 
nofl_verify_certificate(nofl_certificate *cert, const char *pubkey) 
{
  xmlSecKeysMngrPtr mgr;
  xmlSecKeyPtr key;
  xmlNodePtr node;
  xmlSecDSigCtxPtr dsig;
  int result;

  mgr = xmlSecKeysMngrCreate();
  
  xmlSecCryptoAppDefaultKeysMngrInit(mgr);

  key = xmlSecCryptoAppKeyLoad(pubkey, 
			       xmlSecKeyDataFormatPem, 
			       NULL, 
			       NULL, 
			       NULL);
  xmlSecKeySetName(key, pubkey);
  xmlSecCryptoAppDefaultKeysMngrAdoptKey(mgr, key);

  node = xmlSecFindNode(xmlDocGetRootElement(cert->document), 
			xmlSecNodeSignature, 
			xmlSecDSigNs);

  dsig = xmlSecDSigCtxCreate(mgr);
  result = xmlSecDSigCtxVerify(dsig, node);
  if(dsig->status == xmlSecDSigStatusSucceeded)
    result = 0;
  else
    result = -1;

  xmlSecKeysMngrDestroy(mgr);

  return result;
}
