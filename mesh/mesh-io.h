/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2018  Intel Corporation. All rights reserved.
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

struct mesh_io;

#define MESH_IO_FILTER_BEACON	1
#define MESH_IO_FILTER_PROV	2
#define MESH_IO_FILTER_NET	3

#define MESH_IO_TX_COUNT_UNLIMITED	0

enum mesh_io_type {
	MESH_IO_TYPE_NONE = 0,
	MESH_IO_TYPE_GENERIC
};

enum mesh_io_timing_type {
	MESH_IO_TIMING_TYPE_GENERAL = 1,
	MESH_IO_TIMING_TYPE_POLL,
	MESH_IO_TIMING_TYPE_POLL_RSP
};

struct mesh_io_recv_info {
	uint32_t instant;
	uint8_t chan;
	int8_t rssi;
};

struct mesh_io_send_info {
	enum mesh_io_timing_type type;
	union {
		struct {
			uint16_t interval;
			uint8_t cnt;
			uint8_t min_delay;
			uint8_t max_delay;
		} gen;

		struct {
			uint16_t scan_duration;
			uint8_t scan_delay;
			uint8_t filter_ids[2];
			uint8_t min_delay;
			uint8_t max_delay;
		} poll;

		struct {
			uint32_t instant;
			uint8_t delay;
		} poll_rsp;

	} u;
};

struct mesh_io_caps {
	uint8_t max_num_filters;
	uint8_t window_accuracy;
};

typedef void (*mesh_io_recv_func_t)(void *user_data,
					struct mesh_io_recv_info *info,
					const uint8_t *data, uint16_t len);

typedef void (*mesh_io_status_func_t)(void *user_data, int status,
							uint8_t filter_id);

struct mesh_io *mesh_io_new(enum mesh_io_type type, void *opts);
void mesh_io_destroy(struct mesh_io *io);

bool mesh_io_get_caps(struct mesh_io *io, struct mesh_io_caps *caps);

bool mesh_io_register_recv_cb(struct mesh_io *io, uint8_t filter_id,
				mesh_io_recv_func_t cb, void *user_data);

bool mesh_io_deregister_recv_cb(struct mesh_io *io, uint8_t filter_id);

bool mesh_set_filter(struct mesh_io *io, uint8_t filter_id,
				const uint8_t *data, uint8_t len,
				mesh_io_status_func_t cb, void *user_data);

bool mesh_io_send(struct mesh_io *io, struct mesh_io_send_info *info,
					const uint8_t *data, uint16_t len);
bool mesh_io_send_cancel(struct mesh_io *io, const uint8_t *pattern,
								uint8_t len);
