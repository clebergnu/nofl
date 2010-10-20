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

#include <libxml/parser.h>
#include "init.h"

static int initialized;

int nofl_initialize(void) {
  
  int init_error = NOFL_INIT_SUCCESS;

  /* libxml */
  /* FIXME: check xmlInitParser result if possible */
  xmlInitParser();

  /* 
   * Configuration needed for proper xml canonicalization (c14n)
   */
  xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
  xmlSubstituteEntitiesDefault(1);

  /* 
   * FIXME: check if this is needed for proper signature function
   * xmlIndentTreeOutput = 1;
   */

  /* 
   * Initialize xmlsec library 
   */
  if((init_error = xmlSecInit()) < 0)
    return init_error;

  /* 
   * Initialize xmlsec crypto engine (XMLSEC is '-D'efined on build)
   */
  if((init_error = xmlSecCryptoDLLoadLibrary(XMLSEC_CRYPTO)) < 0)
    return init_error;

  if((init_error = xmlSecCryptoAppInit(NULL)) < 0)
    return init_error;

  if((init_error = xmlSecCryptoInit()) < 0)
    return init_error;

  if (init_error == NOFL_INIT_SUCCESS)
    initialized = NOFL_INITIALIZED;

  return init_error;
}

int nofl_shutdown(void) {

  int shutdown_error = NOFL_SHUTDOWN_SUCCESS;

  /* FIXME: capture errors from shutdown into shutdown_error */
  xmlSecCryptoShutdown();
  xmlSecCryptoAppShutdown();
  xmlSecShutdown();
  xmlCleanupParser();

  if (shutdown_error == NOFL_SHUTDOWN_SUCCESS)
    initialized = NOFL_NOT_INITIALIZED;

  return shutdown_error;
}


int 
nofl_initialized(void) {
  return initialized;
}
