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

#include <string.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/xmlsave.h>
#include <libxml/xmlwriter.h>

#include <xmlsec/crypto.h>

#include "certificate.h"

#define NOFL_CERTIFICATE_VERSION "1.0"

/* 
 * Document Encoding (as in <?xml.. encoding="">)
 */
#define ENCODING "UTF-8"

/* 
 * Tags and attributes used in the certificate
 *
 * sample: <nofl-cert version="1.0">
 */
#define TAG_CERTIFICATE             "nofl-cert"
#define ATT_CERTIFICATE_VERSION     "version"

#define TAG_CUSTOMER                "customer"
#define ATT_CUSTOMER_NAME           "name"
#define ATT_CUSTOMER_ID             "identification"

#define TAG_PRODUCT                 "product"
#define ATT_PRODUCT_NAME            "name"
#define ATT_PRODUCT_VERSION         "version"
#define ATT_PRODUCT_MIN_VERSION     "min_version"
#define ATT_PRODUCT_MAX_VERSION     "max_version"

#define TAG_EXPIRE                  "expire"
#define ATT_EXPIRE_INITIAL          "initial"
#define ATT_EXPIRE_FINAL            "final"
#define ATT_EXPIRE_GRACE            "grace"

/* 
 * Attribute value for grace
 */
#define ATT_EXPIRE_GRACE_DAYS      "days"
#define ATT_EXPIRE_GRACE_DAYS_FMT  "%u days"

/* 
 * XPath expressions for locating elements
 */
#define XPATH_TAG_CERTIFICATE       "/nofl-cert"
#define XPATH_TAG_CUSTOMER          "/nofl-cert/customer"
#define XPATH_TAG_PRODUCT           "/nofl-cert/product"
#define XPATH_TAG_EXPIRE            "/nofl-cert/expire"

void
nofl_certificate_save(nofl_certificate *cert) {

  xmlSaveCtxtPtr save_context;
  
  save_context = xmlSaveToFilename(cert->output_path,
				   ENCODING,
				   XML_SAVE_FORMAT);
  xmlSaveDoc(save_context,
	     cert->document);
  xmlSaveClose(save_context);

}

nofl_certificate *
nofl_certificate_new(void) {
  nofl_certificate *cert;

  cert = malloc(sizeof(struct nofl_certificate_t));

  cert->name = NULL;
  cert->key_path = NULL;
  cert->output_path = NULL;

  cert->c14n_method = xmlSecTransformExclC14NId;
  cert->sign_method = xmlSecTransformRsaSha1Id;

}

nofl_certificate *
nofl_certificate_load(const char *path) {
  nofl_certificate *cert;

  cert = nofl_certificate_new();
  cert->document = xmlReadFile(path, ENCODING, 0);

  return cert;
}

nofl_certificate *
nofl_certificate_create(const char *name) { 

  nofl_certificate *cert;
  xmlNodePtr node;

  cert = nofl_certificate_new();
  cert->name = strdup(name);

  /* Start of XML writting */
  cert->document = xmlNewDoc("1.0");
  cert->root_node = xmlNewNode(NULL, TAG_CERTIFICATE);
  xmlDocSetRootElement(cert->document, cert->root_node);
  xmlNewProp(cert->root_node, ATT_CERTIFICATE_VERSION, NOFL_CERTIFICATE_VERSION);

  /* Customer */
  node = xmlNewChild(cert->root_node, NULL, TAG_CUSTOMER, NULL);
  xmlNewProp(node, ATT_CUSTOMER_NAME, NULL);
  xmlNewProp(node, ATT_CUSTOMER_ID, NULL);

  /* Product */
  node = xmlNewChild(cert->root_node, NULL, TAG_PRODUCT, NULL);
  xmlNewProp(node, ATT_PRODUCT_NAME, NULL);
  xmlNewProp(node, ATT_PRODUCT_VERSION, NULL);
  xmlNewProp(node, ATT_PRODUCT_MIN_VERSION, NULL);
  xmlNewProp(node, ATT_PRODUCT_MAX_VERSION, NULL);

  /* Expire */
  node = xmlNewChild(cert->root_node, NULL, TAG_EXPIRE, NULL);
  xmlNewProp(node, ATT_EXPIRE_INITIAL, NULL);
  xmlNewProp(node, ATT_EXPIRE_FINAL, NULL);
  xmlNewProp(node, ATT_EXPIRE_GRACE, NULL);

  return cert;
}

static xmlNodeSetPtr get_nodeset(nofl_certificate *cert, const xmlChar *expr) {

  xmlXPathContextPtr context; 
  xmlXPathObjectPtr object; 
  xmlNodeSetPtr nodeset;

  context = xmlXPathNewContext(cert->document);
  if (context==NULL) {
    printf("!!! ERROR xmlXPathNewContext\n");
    return NULL;
  }

  object = xmlXPathEvalExpression(expr, context);
  if (object==NULL) {
    printf("!!! ERROR xmlXPathEvalExpression\n");
    return;
  }

  nodeset = object->nodesetval;

  return nodeset;
}

static xmlNodePtr get_node(nofl_certificate *cert, const xmlChar *expr, int offset) {

  xmlNodeSetPtr nodeset;
  
  nodeset = get_nodeset(cert, expr);
  if (nodeset && nodeset->nodeNr >= offset) 
    return nodeset->nodeTab[offset];

  return NULL;
}

/* 
 * Gets the version of the certificate
 */
char *nofl_certificate_get_name(nofl_certificate *cert) {

  return strdup(cert->name);
}

/* 
 * Gets the version of the certificate
 */
char *nofl_certificate_get_version(nofl_certificate *cert) {

  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CERTIFICATE, 0);
  if (node)
    return xmlGetProp(node, ATT_CERTIFICATE_VERSION);

  return NULL;
}

/* 
 * Sets the version of the certificate
 */
void nofl_certificate_set_version(nofl_certificate *cert, char *version) {

  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CERTIFICATE, 0);
  if (node)
    xmlSetProp(node, ATT_CERTIFICATE_VERSION, version);
}

/* 
 * Sets the customer name
 */
void nofl_certificate_set_customer_name(nofl_certificate *cert, char *name) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CUSTOMER, 0);
  if (node)
    xmlSetProp(node, ATT_CUSTOMER_NAME, name);
}

/* 
 * Gets the customer name
 */
char *nofl_certificate_get_customer_name(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CUSTOMER, 0);
  if (node)
    return xmlGetProp(node, ATT_CUSTOMER_NAME);

  return NULL;
}

/* 
 * Sets the customer identification
 */
void nofl_certificate_set_customer_id(nofl_certificate *cert, char *id) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CUSTOMER, 0);
  if (node)
    xmlSetProp(node, ATT_CUSTOMER_ID, id);

}

/* 
 * Gets the customer identification
 */
char *nofl_certificate_get_customer_id(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_CUSTOMER, 0);
  if (node)
    return xmlGetProp(node, ATT_CUSTOMER_ID);

  return NULL;
}

/* 
 * Sets the product name
 */
void nofl_certificate_set_product_name(nofl_certificate *cert, char *name) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    xmlSetProp(node, ATT_PRODUCT_NAME, name);

}

/* 
 * Gets the product name
 */
char *nofl_certificate_get_product_name(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    return xmlGetProp(node, ATT_PRODUCT_NAME);

  return NULL;
}

/* 
 * Sets the product version
 */
void nofl_certificate_set_product_version(nofl_certificate *cert, char *version) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    xmlSetProp(node, ATT_PRODUCT_VERSION, version);

}


/* 
 * Sets the product version
 */
char *nofl_certificate_get_product_version(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    return xmlGetProp(node, ATT_PRODUCT_VERSION);

  return NULL;
}


/* 
 * Sets the minimum product version
 */
void nofl_certificate_set_product_min_version(nofl_certificate *cert, char *version) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    xmlSetProp(node, ATT_PRODUCT_MIN_VERSION, version);

}

/* 
 * Gets the minimum product version
 */
char *nofl_certificate_get_product_min_version(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    return xmlGetProp(node, ATT_PRODUCT_MIN_VERSION);

  return NULL;
}

/* 
 * Sets the maximum product version
 */
void nofl_certificate_set_product_max_version(nofl_certificate *cert, char *version) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    xmlSetProp(node, ATT_PRODUCT_MAX_VERSION, version);

}

/* 
 * Gets the maximum product version
 */
char *nofl_certificate_get_product_max_version(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_PRODUCT, 0);
  if (node)
    return xmlGetProp(node, ATT_PRODUCT_MAX_VERSION);

  return NULL;
}

/* 
 * Sets the expire initial date
 */
void nofl_certificate_set_initial_date(nofl_certificate *cert, char *date) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    xmlSetProp(node, ATT_EXPIRE_INITIAL, date);

}

/* 
 * Gets the expire initial date
 */
char *nofl_certificate_get_initial_date(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    return xmlGetProp(node, ATT_EXPIRE_INITIAL);

  return NULL;
}

/* 
 * Sets the expire final date
 */
void nofl_certificate_set_expire_date(nofl_certificate *cert, char *date) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    xmlSetProp(node, ATT_EXPIRE_FINAL, date);

}

/* 
 * Gets the expire final date
 */
char *nofl_certificate_get_expire_date(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    return xmlGetProp(node, ATT_EXPIRE_FINAL);

  return NULL;
}

/* 
 * Sets the grace period (after expire date) in number of days
 */
void nofl_certificate_set_expire_grace(nofl_certificate *cert, int grace) {
  
  xmlNodePtr node;
  char *grace_string;

  sprintf(grace_string, "%i", grace);

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    xmlSetProp(node, ATT_EXPIRE_GRACE, grace_string);

}

/* 
 * Sets the grace period (after expire date) in number of days
 */
char *nofl_certificate_get_expire_grace(nofl_certificate *cert) {
  
  xmlNodePtr node;

  node = get_node(cert, XPATH_TAG_EXPIRE, 0);
  if (node)
    return xmlGetProp(node, ATT_EXPIRE_GRACE);

  return NULL;
}


void 
nofl_certificate_destroy(nofl_certificate *cert) {

  free(cert->name);
  free(cert->key_path);
  free(cert->output_path);
  
  xmlFreeDoc(cert->document);
  
  free(cert);
}

void
nofl_certificate_dump(nofl_certificate *cert) {

  xmlDocFormatDump(stdout, cert->document, 1); 


}

/* 
 * Creates a new writer for xml (xmlTextWriter)
 */
/* static xmlTextWriterPtr create_writer(nofl_certificate *cert) { */
/*   xmlTextWriterPtr writer; */

/*   writer = xmlNewTextWriterDoc(&cert->document, 0); */

/*   xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL); */
/*   xmlTextWriterSetIndent(writer, 1); */

/*   return writer; */
/* } */
