/* ------------------------------
 * SaveMesh plug-in toolbox
 */

#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

// InputChange event handler.
class SMOnInputChangedEventHander: public InputChangedEventHandler
{
public:
    void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
};

// CommandExecuted event handler.
class SMOnExecuteEventHander: public CommandEventHandler
{
public:
    void notify(const Ptr<CommandEventArgs>& eventArgs) override {}
};

// CommandDestroyed event handler
class SMOnDestroyEventHandler : public CommandEventHandler
{
public:
    void notify(const Ptr<CommandEventArgs>& eventArgs) override
    {
        adsk::terminate();
    }
};

// CommandCreated event handler
class SMCommandCreatedEventHandler: public CommandCreatedEventHandler
{
public:
    void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override;
private:
    SMOnExecuteEventHander onExecuteHandler;
    SMOnDestroyEventHandler onDestroyHandler;
    SMOnInputChangedEventHander onInputChangedHandler;
};

extern SMCommandCreatedEventHandler _cmdCreatedHandler;
