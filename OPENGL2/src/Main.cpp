

#include <iostream>

#include "Application.h"
#include "efsw.hpp"


bool modified;

// Inherits from the abstract listener class, and implements the the file action handler
class UpdateListener : public efsw::FileWatchListener {
public:
    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override {
        switch (action) {
        case efsw::Actions::Add:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added"
                << std::endl;
            break;
        case efsw::Actions::Delete:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete"
                << std::endl;
            break;
        case efsw::Actions::Modified:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified"
                << std::endl;
            modified = true;
         
            break;
        case efsw::Actions::Moved:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from ("
                << oldFilename << ")" << std::endl;
            break;
        default:
            std::cout << "Should never happen!" << std::endl;
        }
    }
  
};




int main(int argc, char* argv[])
{
	int fullA = 1920;
	int fullB = 1080;
	int testA = 16*80;
	int testb = 9*80;
	Application* app = new Application(testb,testA,&modified);

    // generic file watcher instead of the platform specific backend
    efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();

    // Create the instance of your efsw::FileWatcherListener implementation
    UpdateListener* listener = new UpdateListener();
   
    // Add a folder to watch, and get the efsw::WatchID
    // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
    // Reporting the files and directories changes to the instance of the listener
    efsw::WatchID watchID = fileWatcher->addWatch("C:/dev/OPENGL/OPENGL2/OPENGL2/OPENGL2/Shaders", listener, true);
  
    // Adds another directory to watch. This time as non-recursive.
    efsw::WatchID watchID2 = fileWatcher->addWatch("/usr", listener, false);

    // Start watching asynchronously the directories
    fileWatcher->watch();

    // Remove the second watcher added
    // You can also call removeWatch by passing the watch path ( it must end with an slash or backslash
    // in windows, since that's how internally it's saved )
    fileWatcher->removeWatch(watchID2);


	if (app->Init())
	{
		app->Run();
	}
	else
	{
		std::cout << " Failed to initialize app" << "/n";
	}
	delete app;
	return 0;
}

