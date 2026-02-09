#include "builtin.h"
#include "gettext.h"
#include "parse-options.h"
#include "run-command.h"
#include "strbuf.h"

static const char * const iaintreadingallat_usage[] = {
	N_("git iaintreadingallat"),
	NULL
};

static const struct option iaintreadingallat_options[] = {
	OPT_END()
};

static int run_git_cmd(struct strvec *args)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushv(&cmd.args, args->v);
	cmd.git_cmd = 1;
	cmd.no_stdin = 1;
	return run_command(&cmd);
}

static int get_current_branch(struct strbuf *branch)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	strvec_pushl(&cmd.args, "symbolic-ref", "--quiet", "--short", "HEAD", NULL);
	cmd.git_cmd = 1;
	cmd.no_stdin = 1;

	if (capture_command(&cmd, branch, 0))
		return -1;

	strbuf_trim_trailing_newline(branch);
	return branch->len ? 0 : -1;
}

int cmd_iaintreadingallat(int argc, const char **argv, const char *prefix,
			  struct repository *repo UNUSED)
{
	struct strvec args = STRVEC_INIT;
	struct strbuf branch = STRBUF_INIT;
	int diff_status;
	int push_status;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	argc = parse_options(argc, argv, prefix, iaintreadingallat_options,
			     iaintreadingallat_usage, 0);
	if (argc)
		usage_with_options(iaintreadingallat_usage,
				   iaintreadingallat_options);

	printf_ln(_("hold tight twin"));

	/* Add everything, including ignored files. */
	strvec_pushl(&args, "add", "-A", "-f", NULL);
	if (run_git_cmd(&args)) {
		strvec_clear(&args);
		return error(_("could not add files"));
	}
	strvec_clear(&args);

	/* Commit if there are staged changes. */
	strvec_pushl(&args, "diff", "--cached", "--quiet", NULL);
	diff_status = run_git_cmd(&args);
	strvec_clear(&args);
	if (diff_status < 0)
		return error(_("could not check staged changes"));
	if (diff_status == 1) {
		strvec_pushl(&args, "commit", "-m", "iaintreadingallat",
			     "--no-verify", NULL);
		if (run_git_cmd(&args)) {
			strvec_clear(&args);
			return error(_("could not commit changes"));
		}
		strvec_clear(&args);
	} else if (diff_status > 1) {
		return error(_("could not check staged changes"));
	}

	/* Push forcefully, skipping hooks. Prefer upstream; fall back to origin. */
	strvec_pushl(&args, "push", "--force", "--no-verify", NULL);
	push_status = run_git_cmd(&args);
	strvec_clear(&args);

	if (push_status && !get_current_branch(&branch)) {
		strvec_pushl(&args, "push", "--force", "--no-verify",
			     "origin", branch.buf, NULL);
		push_status = run_git_cmd(&args);
		strvec_clear(&args);
	}

	strbuf_release(&branch);
	return push_status;
}
