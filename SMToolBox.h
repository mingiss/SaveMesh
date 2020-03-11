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
class SmOnInputChangedEventHander: public InputChangedEventHandler
{
public:
    void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
};

// CommandExecuted event handler.
class SmOnExecuteEventHander: public CommandEventHandler
{
public:
    void notify(const Ptr<CommandEventArgs>& eventArgs) override {}
};

// CommandDestroyed event handler
class SmOnDestroyEventHandler : public CommandEventHandler
{
public:
    void notify(const Ptr<CommandEventArgs>& eventArgs) override
    {
        adsk::terminate();
    }
};

// CommandCreated event handler
class SmToolboxHandler: public CommandCreatedEventHandler
{
public:
    Ptr<UserInterface> m_ui = NULL;

    bool init(Ptr<UserInterface> ui);
    void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override;
private:
    SmOnExecuteEventHander onExecuteHandler;
    SmOnDestroyEventHandler onDestroyHandler;
    SmOnInputChangedEventHander onInputChangedHandler;
};

extern SmToolboxHandler theSmToolBoxHandler;
