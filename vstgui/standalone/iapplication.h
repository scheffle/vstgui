#pragma once

#include "fwd.h"
#include "interface.h"
#include "iwindow.h"
#include <vector>
#include <cstdint>

//------------------------------------------------------------------------
namespace VSTGUI {
namespace Standalone {

//------------------------------------------------------------------------
class IApplication : public Interface
{
public:
	using WindowList = std::vector<WindowPtr>;

	static IApplication& instance ();

	virtual Application::IDelegate* getDelegate () const = 0;
	virtual WindowPtr createWindow (const WindowConfiguration& config, const WindowControllerPtr& controller) = 0;
	virtual const WindowList& getWindows () const = 0;
	
	virtual void registerCommand (const Command& command, char16_t defaultCommandKey) = 0;
	virtual void quit () = 0;
};

//------------------------------------------------------------------------
} // Standalone
} // VSTGUI