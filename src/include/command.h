#pragma once
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/**
 * $Id$
 *
 * @file include/command.h
 * @brief Structures and prototypes command functions
 *
 * @copyright 2007  Alan DeKok
 */
RCSIDH(command_h, "$Id$")

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_MAX_ARGV (32)

typedef struct fr_cmd_t fr_cmd_t;

typedef int (*fr_cmd_func_t)(FILE *fp, void *ctx, int argc, char const *argv[]);

typedef struct fr_cmd_tab_info_t {
	int		argc;
	char const	**argv;				//!< last argument is to be expanded
} fr_cmd_tab_info_t;

typedef int (*fr_cmd_tab_t)(TALLOC_CTX *talloc_ctx, void *ctx, fr_cmd_tab_info_t *info, int max_expansions, char const **expansions);

typedef struct fr_cmd_table_t {
	char const		**parents;		//!< e.g. "show module"
	char const		*syntax;		//!< e.g. "stats FOO"
	char const		*help;			//!< help text
	fr_cmd_func_t		func;			//!< function to process this command
	fr_cmd_tab_t		tab_expand;		//!< tab expand things in the syntax string
	bool			read_only;
} fr_cmd_table_t;

#define CMD_TABLE_END { .help = NULL }

typedef struct fr_cmd_walk_info_t {
	int		num_parents;
	char const	**parents;
	char const	*name;
	char const	*syntax;
	char const	*help;
} fr_cmd_walk_info_t;

typedef int (*fr_cmd_walk_t)(void *ctx, fr_cmd_walk_info_t *);

int fr_command_add(TALLOC_CTX *talloc_ctx, fr_cmd_t **head_p, char const *name, void *ctx, fr_cmd_table_t const *table);
int fr_command_add_multi(TALLOC_CTX *talloc_ctx, fr_cmd_t **heap_p, char const *name, void *ctx, fr_cmd_table_t const *table);
int fr_command_walk(fr_cmd_t *head, void **walk_ctx, void *ctx, fr_cmd_walk_t callback);
int fr_command_tab_expand(TALLOC_CTX *ctx, fr_cmd_t *head, int argc, char const *argv[], int max_expansions, char const **expansions);
char const *fr_command_help(fr_cmd_t *head, int argc, char const *argv[]);
int fr_command_run(FILE *fp, fr_cmd_t *head, int argc, char const *argv[]);
void fr_command_debug(FILE *fp, fr_cmd_t *head);
int fr_command_str_to_argv(UNUSED fr_cmd_t *head, int argc, char *argv[], int max_argc, char *str, bool *runnable);

#ifdef __cplusplus
}
#endif
