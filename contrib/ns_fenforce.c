/*
 * Copyright (c) 2005-2007 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the NickServ FENFORCE function.
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/fenforce", false, _modinit, _moddeinit,
	"$Id$",
	"Atheme Development Group <http://www.atheme.org>"
);

static void ns_cmd_fenforce(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_fenforce = { "FENFORCE", "Enables or disables protection of another user's nicknames.", PRIV_USER_ADMIN, 2, ns_cmd_fenforce };

list_t *ns_cmdtree, *ns_helptree;

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(ns_cmdtree, "nickserv/main", "ns_cmdtree");
	MODULE_USE_SYMBOL(ns_helptree, "nickserv/main", "ns_helptree");
	MODULE_TRY_REQUEST_DEPENDENCY(m, "nickserv/enforce");

	command_add(&ns_fenforce, ns_cmdtree);
	help_addentry(ns_helptree, "FENFORCE", "help/contrib/fenforce", NULL);
}

void _moddeinit()
{
	command_delete(&ns_fenforce, ns_cmdtree);
	help_delentry(ns_helptree, "FENFORCE");
}

static void ns_cmd_fenforce(sourceinfo_t *si, int parc, char *parv[])
{
	metadata_t *md;
	char *setting;
	myuser_t *mu;

	if (parc < 2)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "FENFORCE");
		command_fail(si, fault_needmoreparams, _("Syntax: FENFORCE <account> ON|OFF"));
		return;
	}

	mu = myuser_find_ext(parv[0]);
	if (!mu)
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is not registered."), parv[0]);
		return;
	}
	setting = parv[1];

	if (strcasecmp(setting, "ON") == 0)
	{
		if ((md = metadata_find(mu, "private:doenforce")) != NULL)
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for account \2%s\2."), "ENFORCE", mu->name);
		}
		else
		{
			wallops("%s enabled ENFORCE on the account \2%s\2.", get_oper_name(si), mu->name);
			logcommand(si, CMDLOG_ADMIN, "FENFORCE:ON: \2%s\2", mu->name);
			metadata_add(mu, "private:doenforce", "1");
			command_success_nodata(si, _("The \2%s\2 flag has been set for account \2%s\2."), "ENFORCE", mu->name);
		}
	}
	else if (strcasecmp(setting, "OFF") == 0)
	{
		if ((md = metadata_find(mu, "private:doenforce")) != NULL)
		{
			wallops("%s disabled ENFORCE on the account \2%s\2.", get_oper_name(si), mu->name);
			logcommand(si, CMDLOG_ADMIN, "FENFORCE:OFF: \2%s\2", mu->name);
			metadata_delete(mu, "private:doenforce");
			command_success_nodata(si, _("The \2%s\2 flag has been removed for account \2%s\2."), "ENFORCE", mu->name);
		}
		else
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for account \2%s\2."), "ENFORCE", mu->name);
		}
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "FENFORCE");
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
