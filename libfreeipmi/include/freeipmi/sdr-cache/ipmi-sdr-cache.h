/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
/*****************************************************************************\
 *  Copyright (C) 2007-2012 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2006-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-222073
 *
 *  This file is part of Ipmimonitoring, an IPMI sensor monitoring
 *  library.  For details, see http://www.llnl.gov/linux/.
 *
 *  Ipmimonitoring is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Ipmimonitoring is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmimonitoring.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef IPMI_SDR_CACHE_H
#define IPMI_SDR_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freeipmi/api/ipmi-api.h>

#define IPMI_SDR_ERR_SUCCESS                                      0
#define IPMI_SDR_ERR_CONTEXT_NULL                                 1
#define IPMI_SDR_ERR_CONTEXT_INVALID                              2
#define IPMI_SDR_ERR_PARAMETERS                                   3
#define IPMI_SDR_ERR_OUT_OF_MEMORY                                4
#define IPMI_SDR_ERR_FILENAME_INVALID                             5
#define IPMI_SDR_ERR_FILESYSTEM                                   6
#define IPMI_SDR_ERR_PERMISSION                                   7
#define IPMI_SDR_ERR_CONTEXT_PERFORMING_OTHER_OPERATION           8
#define IPMI_SDR_ERR_CACHE_CREATE_CACHE_EXISTS                    9
#define IPMI_SDR_ERR_CACHE_CREATE_DUPLICATE_RECORD_ID             10
#define IPMI_SDR_ERR_CACHE_CREATE_INVALID_RECORD_LENGTH           11
#define IPMI_SDR_ERR_CACHE_CREATE_INVALID_RECORD_COUNT            12
#define IPMI_SDR_ERR_CACHE_READ_ALREADY_INITIALIZED               13
#define IPMI_SDR_ERR_CACHE_READ_INITIALIZATION                    14
#define IPMI_SDR_ERR_CACHE_READ_CACHE_DOES_NOT_EXIST              15
#define IPMI_SDR_ERR_CACHE_INVALID                                16
#define IPMI_SDR_ERR_CACHE_OUT_OF_DATE                            17
#define IPMI_SDR_ERR_NOT_FOUND                                    18
#define IPMI_SDR_ERR_IPMI_ERROR                                   19
#define IPMI_SDR_ERR_SYSTEM_ERROR                                 20
#define IPMI_SDR_ERR_OVERFLOW                                     21
#define IPMI_SDR_ERR_INTERNAL_ERROR                               22
#define IPMI_SDR_ERR_ERRNUMRANGE                                  23

#define IPMI_SDR_FLAGS_DEFAULT                   0x0000
#define IPMI_SDR_FLAGS_DEBUG_DUMP                0x0001

/* Flags just for cache creation
 *
 * OVERWRITE - overwrite any previously created cache.
 *
 * DUPLICATE_RECORD_ID - check for duplicate record IDs and return error if one is found. 
 */
#define IPMI_SDR_CACHE_CREATE_FLAGS_DEFAULT             0x0
#define IPMI_SDR_CACHE_CREATE_FLAGS_OVERWRITE           0x1
#define IPMI_SDR_CACHE_CREATE_FLAGS_DUPLICATE_RECORD_ID 0x2

#define IPMI_SDR_CACHE_MAX_SDR_RECORD_LENGTH            261 /* 256 + header */

typedef struct ipmi_sdr_ctx *ipmi_sdr_ctx_t;

/* Callback between every record that is cached */
typedef void (*Ipmi_Sdr_Create_Callback)(uint8_t sdr_version,
					 uint16_t record_count,
					 uint32_t most_recent_addition_timestamp,
					 uint32_t most_recent_erase_timestamp,
					 uint16_t record_id,
					 void *data);

/*
 * SDR Cache Context and General functions
 */
ipmi_sdr_ctx_t ipmi_sdr_cache_ctx_create (void);
void ipmi_sdr_cache_ctx_destroy (ipmi_sdr_ctx_t ctx);
int ipmi_sdr_cache_ctx_errnum (ipmi_sdr_ctx_t ctx);
char * ipmi_sdr_cache_ctx_strerror (int errnum);
char * ipmi_sdr_cache_ctx_errormsg (ipmi_sdr_ctx_t ctx);

int ipmi_sdr_cache_ctx_get_flags (ipmi_sdr_ctx_t ctx, unsigned int *flags);
int ipmi_sdr_cache_ctx_set_flags (ipmi_sdr_ctx_t ctx, unsigned int flags);
char *ipmi_sdr_cache_ctx_get_debug_prefix (ipmi_sdr_ctx_t ctx);
int ipmi_sdr_cache_ctx_set_debug_prefix (ipmi_sdr_ctx_t ctx, const char *debug_prefix);

/*
 * SDR Cache Creation Functions
 */
int ipmi_sdr_cache_create (ipmi_sdr_ctx_t ctx,
                           ipmi_ctx_t ipmi_ctx,
                           const char *filename,
                           int cache_create_flags,
                           Ipmi_Sdr_Create_Callback create_callback,
                           void *create_callback_data);

/*
 * SDR Cache Reading Functions
 */

/* ipmi_sdr_cache_open
 *  - ipmi_ctx is optional
 * - if ipmi_ctx is specified, timestamps/version of the SDR will be
 *   checked for out-of-dateness
 */
int ipmi_sdr_cache_open (ipmi_sdr_ctx_t ctx,
                         ipmi_ctx_t ipmi_ctx,
                         const char *filename);

int ipmi_sdr_cache_sdr_version (ipmi_sdr_ctx_t ctx, uint8_t *sdr_version);
int ipmi_sdr_cache_record_count (ipmi_sdr_ctx_t ctx, uint16_t *record_count);
int ipmi_sdr_cache_most_recent_addition_timestamp (ipmi_sdr_ctx_t ctx,
                                                   uint32_t *most_recent_addition_timestamp);
int ipmi_sdr_cache_most_recent_erase_timestamp (ipmi_sdr_ctx_t ctx,
                                                uint32_t *most_recent_erase_timestamp);

int ipmi_sdr_cache_first (ipmi_sdr_ctx_t ctx);
int ipmi_sdr_cache_next (ipmi_sdr_ctx_t ctx);
int ipmi_sdr_cache_seek (ipmi_sdr_ctx_t ctx, unsigned int index);
int ipmi_sdr_cache_search_record_id (ipmi_sdr_ctx_t ctx, uint16_t record_id);
/* sensor owner id is 8bit field - 7 bit slave or system software id + 1 bit indicating type */
int ipmi_sdr_cache_search_sensor (ipmi_sdr_ctx_t ctx, uint8_t sensor_number, uint8_t sensor_owner_id);

/* return length of data read into buffer on success, -1 on error */
int ipmi_sdr_cache_record_read (ipmi_sdr_ctx_t ctx,
                                void *buf,
                                unsigned int buflen);

int ipmi_sdr_cache_close (ipmi_sdr_ctx_t ctx);

/*
 * SDR Cache Utility Functions
 */
int ipmi_sdr_cache_delete (ipmi_sdr_ctx_t ctx, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* IPMI_SDR_CACHE_H */
