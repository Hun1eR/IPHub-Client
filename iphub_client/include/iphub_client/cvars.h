// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <cssdk/common/cvar.h>

/// <summary>
/// </summary>
extern CVar* iphub_log;

/// <summary>
/// </summary>
inline bool get_iphub_log()
{
	return static_cast<bool>(iphub_log->value);
}

/// <summary>
/// </summary>
bool register_cvars();
