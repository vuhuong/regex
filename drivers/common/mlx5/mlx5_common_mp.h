/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2018 6WIND S.A.
 * Copyright 2018 Mellanox Technologies, Ltd
 */

#ifndef RTE_PMD_MLX5_COMMON_MP_H_
#define RTE_PMD_MLX5_COMMON_MP_H_

/* Verbs header. */
/* ISO C doesn't support unnamed structs/unions, disabling -pedantic. */
#ifdef PEDANTIC
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#include <infiniband/verbs.h>
#ifdef PEDANTIC
#pragma GCC diagnostic error "-Wpedantic"
#endif

#include <rte_string_fns.h>

/* Request types for IPC. */
enum mlx5_mp_req_type {
	MLX5_MP_REQ_VERBS_CMD_FD = 1,
	MLX5_MP_REQ_CREATE_MR,
	MLX5_MP_REQ_START_RXTX,
	MLX5_MP_REQ_STOP_RXTX,
	MLX5_MP_REQ_QUEUE_STATE_MODIFY,
};

struct mlx5_mp_arg_queue_state_modify {
	uint8_t is_wq; /* Set if WQ. */
	uint16_t queue_id; /* DPDK queue ID. */
	enum ibv_wq_state state; /* WQ requested state. */
};

/* Pameters for IPC. */
struct mlx5_mp_param {
	enum mlx5_mp_req_type type;
	int port_id;
	int result;
	RTE_STD_C11
	union {
		uintptr_t addr; /* MLX5_MP_REQ_CREATE_MR */
		struct mlx5_mp_arg_queue_state_modify state_modify;
		/* MLX5_MP_REQ_QUEUE_STATE_MODIFY */
	} args;
};

/** Request timeout for IPC. */
#define MLX5_MP_REQ_TIMEOUT_SEC 5

/** Key string for IPC. */
#define MLX5_MP_NAME "net_mlx5_mp"

/**
 * Initialize IPC message.
 *
 * @param[in] port_id
 *   Port ID of the device.
 * @param[out] msg
 *   Pointer to message to fill in.
 * @param[in] type
 *   Message type.
 */
static inline void
mp_init_msg(uint16_t port_id, struct rte_mp_msg *msg,
	    enum mlx5_mp_req_type type)
{
	struct mlx5_mp_param *param = (struct mlx5_mp_param *)msg->param;

	memset(msg, 0, sizeof(*msg));
	strlcpy(msg->name, MLX5_MP_NAME, sizeof(msg->name));
	msg->len_param = sizeof(*param);
	param->type = type;
	param->port_id = port_id;
}

__rte_experimental
int mlx5_mp_init_primary(rte_mp_t primary_action);
__rte_experimental
void mlx5_mp_uninit_primary(void);
__rte_experimental
int mlx5_mp_init_secondary(rte_mp_t secondary_action);
__rte_experimental
void mlx5_mp_uninit_secondary(void);
__rte_experimental
int mlx5_mp_req_mr_create(uint16_t port_id, uintptr_t addr);
__rte_experimental
int mlx5_mp_req_queue_state_modify(uint16_t port_id,
				   struct mlx5_mp_arg_queue_state_modify *sm);
__rte_experimental
int mlx5_mp_req_verbs_cmd_fd(uint16_t port_id);

#endif /* RTE_PMD_MLX5_COMMON_MP_H_ */
