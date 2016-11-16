#pragma once

#include "interface.h"
#include "../../lib/cstring.h"

//------------------------------------------------------------------------
namespace VSTGUI {
namespace Standalone {

//------------------------------------------------------------------------
enum class CommonDirectoryLocation
{
	AppPath,
	AppPreferencesPath,
	AppCachesPath,
	UserDocumentsPath,
};

//------------------------------------------------------------------------
class ICommonDirectories : public Interface
{
public:
	/** Get a common directory.
	 *
	 *	@param location the location of the directory
	 *	@param subDir optional sub directory
	 *	@param create create directory if it does not exist
	 *	@return path string. If location does not exist the string is empty.
	 *						 If it exists, the last character in the path is a separator.
	 */
	virtual UTF8String get (CommonDirectoryLocation location, const UTF8String& subDir = "", bool create = false) const = 0;
};

//------------------------------------------------------------------------
} // Standalone
} // VSTGUI