#pragma once

#include "../imenubuilder.h"

//------------------------------------------------------------------------
namespace VSTGUI {
namespace Standalone {

//------------------------------------------------------------------------
/** Menu builder adapter
 *
 *	@ingroup standalone
 */
class MenuBuilderAdapter : public IMenuBuilder
{
public:
	bool showCommandGroupInMenu (const Interface& context, const UTF8String& group) const override
	{
		return true;
	}

	bool showCommandInMenu (const Interface& context, const Command& cmd) const override
	{
		return true;
	}

	SortFunction getCommandGroupSortFunction (const Interface& context,
	                                          const UTF8String& group) const override
	{
		return {};
	}

	bool prependMenuSeparator (const Interface& context, const Command& cmd) const override
	{
		return false;
	}
};

//------------------------------------------------------------------------
/** No menu builder adapter
 *
 *	Use this to prevent a window to have a menu
 *
 *	@ingroup standalone
 */
class NoMenuBuilder : public MenuBuilderAdapter
{
public:
	bool showCommandGroupInMenu (const Interface& context, const UTF8String& group) const override
	{
		return false;
	}

	bool showCommandInMenu (const Interface& context, const Command& cmd) const override
	{
		return false;
	}
};

//------------------------------------------------------------------------
} // Standalone
} // VSTGUI