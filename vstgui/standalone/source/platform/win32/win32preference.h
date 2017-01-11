#pragma once

#include "../../../include/ipreference.h"
#include <windows.h>

//------------------------------------------------------------------------
namespace VSTGUI {
namespace Standalone {
namespace Platform {
namespace Win32 {

//------------------------------------------------------------------------
class Win32Preference : public IPreference
{
public:
	Win32Preference ();
	~Win32Preference ();

	bool set (const UTF8String& key, const UTF8String& value) override;
	Optional<UTF8String> get (const UTF8String& key) override;

private:
	HKEY hKey;
};

//------------------------------------------------------------------------
} // Mac
} // Platform
} // Standalone
} // VSTGUI