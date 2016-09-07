#include "mandelbrotwindow.h"
#include "vstgui/standalone/helpers/appdelegate.h"
#include "vstgui/standalone/helpers/windowlistener.h"
#include "vstgui/standalone/iapplication.h"
#include "vstgui/standalone/iwindow.h"
#include "vstgui/standalone/icommand.h"
#include <atomic>

//------------------------------------------------------------------------
namespace Mandelbrot {
using namespace VSTGUI;
using namespace VSTGUI::Standalone;
using namespace VSTGUI::Standalone::Application;

//------------------------------------------------------------------------
struct AppDelegate : DelegateAdapter, WindowListenerAdapter, ICommandHandler
{
	AppDelegate () : DelegateAdapter ({"mandelbrot", "1.0.0", "com.mycompany.mandelbrot"}) {}

	void finishLaunching () override
	{
		IApplication::instance ().registerCommand(Commands::NewDocument, 'n');
		if (auto window = makeMandelbrotWindow ())
		{
			window->show ();
			window->registerWindowListener (this);
		}
		else
		{
			IApplication::instance ().quit ();
		}
	}

	bool canHandleCommand (const Command& command) override
	{
		if (command == Commands::NewDocument)
			return true;
		return false;
	}

	bool handleCommand (const Command& command) override
	{
		if (command == Commands::NewDocument)
		{
			if (auto window = makeMandelbrotWindow ())
			{
				window->show ();
				window->registerWindowListener (this);
				return true;
			}
		}
		return false;
	}

	void onClosed (const IWindow& window) override
	{
		if (IApplication::instance ().getWindows ().empty ())
			IApplication::instance ().quit ();
	}
};

static Init gAppDelegate (std::make_unique<AppDelegate> ());

//------------------------------------------------------------------------
} // Mandelbrot