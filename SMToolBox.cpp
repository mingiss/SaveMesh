/* ------------------------------
 * SaveMesh plug-in toolbox 
 */

#include <iostream> 
#include <sstream>

#include "SMToolBox.h"

using namespace std;


// Adds a new row to the table.
static void addRowToTable(Ptr<TableCommandInput> tableInput)
{
	if (!tableInput)
		return;

	// Define a unique id for each row.
	static int rowNumber = 0;
	stringstream rowId;
	rowId << rowNumber;

	// Get the CommandInputs object associated with the parent command.
	Ptr<CommandInputs> cmdInputs = tableInput->commandInputs();

	// Create three new command inputs.
	Ptr<CommandInput> childTableValueInput = cmdInputs->addValueInput("TableInput_value" + rowId.str(), "Value", "cm", ValueInput::createByReal(rowNumber));
	Ptr<CommandInput> childTableStringInput = cmdInputs->addStringValueInput("TableInput_string" + rowId.str(), "String", rowId.str());
	Ptr<CommandInput> childTableSpinnerInput = cmdInputs->addIntegerSpinnerCommandInput("spinnerInt" + rowId.str(), "Integer Spinner", 0, 100, 2, rowNumber);

	// Add the inputs to the table.
	int row = tableInput->rowCount();
	tableInput->addCommandInput(childTableValueInput, row, 0);
	tableInput->addCommandInput(childTableStringInput, row, 1);
	tableInput->addCommandInput(childTableSpinnerInput, row, 2);

	rowNumber = rowNumber + 1;
}


// InputChange event handler.
void SMOnInputChangedEventHander::notify(const Ptr<InputChangedEventArgs>& eventArgs)
{
	Ptr<CommandInputs> inputs = eventArgs->inputs();
	if (!inputs)
		return;

	Ptr<CommandInput> cmdInput = eventArgs->input();
	if (!cmdInput)
		return;

	Ptr<TableCommandInput> tableInput = inputs->itemById("table");
	if (!tableInput)
		return;

	if (!theCmdCreatedHandler.m_ui)
		return;

	if (cmdInput->id() == "tableAdd") {
		addRowToTable(tableInput);
	}
	else if (cmdInput->id() == "tableDelete") {
		if (tableInput->selectedRow() == -1) {
			theCmdCreatedHandler.m_ui->messageBox("Select one row to delete.");
		}
		else {
			tableInput->deleteRow(tableInput->selectedRow());
		}
	}
}


// CommandCreated event handler
bool SMCommandCreatedEventHandler::init(Ptr<UserInterface> ui)
{
	m_ui = ui;
	if (!m_ui)
		return false;

	// Create the command definition.
	Ptr<CommandDefinitions> commandDefinitions = m_ui->commandDefinitions();
	if (!commandDefinitions)
		return nullptr;

	// Get the existing command definition or create it if it doesn't already exist.
	Ptr<CommandDefinition> cmdDef = commandDefinitions->itemById("cmdInputsSample");
	if (!cmdDef)
	{
		cmdDef = commandDefinitions->addButtonDefinition("cmdInputsSample",
			"Command Inputs Sample",
			"Sample to demonstrate various command inputs.");
	}

	// Connect to the command created event.
	Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	if (!commandCreatedEvent)
		return false;
	commandCreatedEvent->add(&theCmdCreatedHandler);

	// Execute the command definition.
	cmdDef->execute();

	// Prevent this module from being terminated when the script returns, because we are waiting for event handlers to fire.
	adsk::autoTerminate(false);

	return true;
}


void SMCommandCreatedEventHandler::notify(const Ptr<CommandCreatedEventArgs>& eventArgs)
{
	if (eventArgs)
	{
		// Get the command that was created.
		Ptr<Command> command = eventArgs->command();
		if (command)
		{
			// Connect to the command destroyed event.
			Ptr<CommandEvent> onDestroy = command->destroy();
			if (!onDestroy)
				return;
			bool isOk = onDestroy->add(&onDestroyHandler);
			if (!isOk)
				return;

			// Connect to the input changed event.
			Ptr<InputChangedEvent> onInputChanged = command->inputChanged();
			if (!onInputChanged)
				return;
			isOk = onInputChanged->add(&onInputChangedHandler);
			if (!isOk)
				return;

			// Get the CommandInputs collection associated with the command.
			Ptr<CommandInputs> inputs = command->commandInputs();
			if (!inputs)
				return;

			// Create a tab input.
			Ptr<TabCommandInput> tabCmdInput1 = inputs->addTabCommandInput("tab_1", "Tab 1");
			if (!tabCmdInput1)
				return;
			Ptr<CommandInputs> tab1ChildInputs = tabCmdInput1->children();
			if (!tab1ChildInputs)
				return;

			// Create a read only textbox input.
			tab1ChildInputs->addTextBoxCommandInput("readonly_textBox", "Text Box 1", "This is an example of a read-only text box.", 2, true);

			// Create an editable textbox input.
			tab1ChildInputs->addTextBoxCommandInput("writable_textBox", "Text Box 2", "This is an example of an editable text box.", 2, false);

			// Create a message that spans the entire width of the dialog by leaving out the "name" argument.
			string message = "<div align=\"center\">A full width message using <a href=\"http:fusion360.autodesk.com\">html.</a></div>";
			tab1ChildInputs->addTextBoxCommandInput("fullWidth_textBox", "", message, 1, true);

			// Create a selection input.
			Ptr<SelectionCommandInput> selectionInput = tab1ChildInputs->addSelectionInput("selection", "Select", "Basic select command input");
			if (!selectionInput)
				return;
			selectionInput->setSelectionLimits(0);

			// Create string value input.
			Ptr<StringValueCommandInput> strInput = tab1ChildInputs->addStringValueInput("string", "Text", "Basic string command input");

			// Create value input.
			tab1ChildInputs->addValueInput("value", "Value", "cm", ValueInput::createByReal(0.0));

			// Create bool value input with checkbox style.
			tab1ChildInputs->addBoolValueInput("checkbox", "Checkbox", true, "", false);

			// Create bool value input with button style that can be clicked.
			tab1ChildInputs->addBoolValueInput("buttonClick", "Click Button", false, "resources/button", true);

			// Create bool value input with button style that has a state.
			tab1ChildInputs->addBoolValueInput("buttonState", "State Button", true, "resources/button", true);

			// Create float slider input with two sliders.
			tab1ChildInputs->addFloatSliderCommandInput("floatSlider", "Float Slider", "cm", 0, 10.0, true);

			// Create float slider input with two sliders and a value list
			vector<double> floatValueList;
			floatValueList.push_back(1.0);
			floatValueList.push_back(3.0);
			floatValueList.push_back(4.0);
			floatValueList.push_back(7.0);
			tab1ChildInputs->addFloatSliderListCommandInput("floatSlider2", "Float Slider 2", "cm", floatValueList);

			// Create float slider input with two sliders and visible texts
			Ptr<FloatSliderCommandInput> floatSlider3 = tab1ChildInputs->addFloatSliderCommandInput("floatSlider3", "Float Slider 3", "", 0, 50.0, false);
			if (!floatSlider3)
				return;
			floatSlider3->setText("Min", "Max");

			// Create integer slider input with one slider
			tab1ChildInputs->addIntegerSliderCommandInput("intSlider", "Integer Slider", 0, 10);

			// Create integer slider input with two sliders and a value list
			vector<int> valueList;
			valueList.push_back(1);
			valueList.push_back(3);
			valueList.push_back(4);
			valueList.push_back(7);
			valueList.push_back(11);
			tab1ChildInputs->addIntegerSliderListCommandInput("intSlider2", "Integer Slider 2", valueList);

			// Create float spinner input.
			tab1ChildInputs->addFloatSpinnerCommandInput("spinnerFloat", "Float Spinner", "cm", 0.2, 9.0, 2.2, 1);

			// Create integer spinner input.
			tab1ChildInputs->addIntegerSpinnerCommandInput("spinnerInt", "Integer Spinner", 2, 9, 2, 3);

			// Create dropdown input with checkbox style.
			Ptr<DropDownCommandInput> dropdownInput = tab1ChildInputs->addDropDownCommandInput("dropdown", "Dropdown 1", DropDownStyles::CheckBoxDropDownStyle);
			if (!dropdownInput)
				return;
			Ptr<ListItems> dropdownItems = dropdownInput->listItems();
			if (!dropdownItems)
				return;
			dropdownItems->add("Item 1", false, "resources/One");
			dropdownItems->add("Item 2", false, "resources/Two");

			// Create dropdown input with icon style.
			Ptr<DropDownCommandInput> dropdownInput2 = tab1ChildInputs->addDropDownCommandInput("dropdown2", "Dropdown 2", DropDownStyles::LabeledIconDropDownStyle);
			if (!dropdownInput2)
				return;
			Ptr<ListItems> dropdown2Items = dropdownInput2->listItems();
			if (!dropdown2Items)
				return;
			dropdown2Items->add("Item 1", true, "resources/One");
			dropdown2Items->add("Item 2", false, "resources/Two");

			// Create dropdown input with radio style.
			Ptr<DropDownCommandInput> dropdownInput3 = tab1ChildInputs->addDropDownCommandInput("dropdown3", "Dropdown 3", DropDownStyles::LabeledIconDropDownStyle);
			if (!dropdownInput3)
				return;
			Ptr<ListItems> dropdown3Items = dropdownInput3->listItems();
			if (!dropdown3Items)
				return;
			dropdown3Items->add("Item 1", true, "");
			dropdown3Items->add("Item 2", false, "");

			// Create dropdown input with test list style.
			Ptr<DropDownCommandInput> dropdownInput4 = tab1ChildInputs->addDropDownCommandInput("dropdown4", "Dropdown 4", DropDownStyles::TextListDropDownStyle);
			if (!dropdownInput4)
				return;
			Ptr<ListItems> dropdown4Items = dropdownInput4->listItems();
			if (!dropdown4Items)
				return;
			dropdown4Items->add("Item 1", true, "");
			dropdown4Items->add("Item 2", false, "");

			// Create single selectable button row input.
			Ptr<ButtonRowCommandInput> buttonRowInput = tab1ChildInputs->addButtonRowCommandInput("buttonRow", "Button Row 1", false);
			if (!buttonRowInput)
				return;
			Ptr<ListItems> buttonRowItems = buttonRowInput->listItems();
			if (!buttonRowItems)
				return;
			buttonRowItems->add("Item 1", false, "resources/One");
			buttonRowItems->add("Item 2", false, "resources/Two");

			// Create multi selectable button row input.
			Ptr<ButtonRowCommandInput> buttonRowInput2 = tab1ChildInputs->addButtonRowCommandInput("buttonRow2", "Button Row 2", true);
			if (!buttonRowInput2)
				return;
			Ptr<ListItems> buttonRow2Items = buttonRowInput2->listItems();
			if (!buttonRow2Items)
				return;
			buttonRow2Items->add("Item 1", false, "resources/One");
			buttonRow2Items->add("Item 2", false, "resources/Two");

			// Create tab input 2.
			Ptr<TabCommandInput> tabCmdInput2 = inputs->addTabCommandInput("tab_2", "Tab 2");
			if (!tabCmdInput2)
				return;
			Ptr<CommandInputs> tab2ChildInputs = tabCmdInput2->children();
			if (!tab2ChildInputs)
				return;

			// Create group input.
			Ptr<GroupCommandInput> groupCmdInput = tab2ChildInputs->addGroupCommandInput("group", "Group");
			if (!groupCmdInput)
				return;
			groupCmdInput->isExpanded(true);
			groupCmdInput->isEnabledCheckBoxDisplayed(true);
			Ptr<CommandInputs> groupChildInputs = groupCmdInput->children();
			if (!groupChildInputs)
				return;

			// Create radio button group input.
			Ptr<RadioButtonGroupCommandInput> radioButtonGroup = groupChildInputs->addRadioButtonGroupCommandInput("radioButtonGroup", "Radio button group");
			if (!radioButtonGroup)
				return;
			Ptr<ListItems> radioButtonItems = radioButtonGroup->listItems();
			if (!radioButtonItems)
				return;
			radioButtonItems->add("Item 1", false);
			radioButtonItems->add("Item 2", false);
			radioButtonItems->add("Item 3", false);

			// Create image input
			Ptr<ImageCommandInput> imageCmdInput = groupChildInputs->addImageCommandInput("image", "Image", "resources/image.png");
			if (!imageCmdInput)
				return;

			// Create direction input 1.
			Ptr<DirectionCommandInput> directionCmdInput = tab2ChildInputs->addDirectionCommandInput("direction", "Direction");
			if (!directionCmdInput)
				return;
			directionCmdInput->setManipulator(Point3D::create(0, 0, 0), Vector3D::create(1, 0, 0));

			// Create direction input 2.
			Ptr<DirectionCommandInput> directionCmdInput2 = tab2ChildInputs->addDirectionCommandInput("direction2", "Direction 2", "resources/One");
			if (!directionCmdInput2)
				return;
			directionCmdInput2->setManipulator(Point3D::create(0, 0, 0), Vector3D::create(0, 1, 0));
			directionCmdInput2->isDirectionFlipped(true);

			// Create distance value input 1.
			Ptr<DistanceValueCommandInput> distanceValueInput = tab2ChildInputs->addDistanceValueCommandInput("distanceValue", "Distance Value", ValueInput::createByReal(2));
			if (!distanceValueInput)
				return;
			distanceValueInput->setManipulator(Point3D::create(0, 0, 0), Vector3D::create(1, 0, 0));
			distanceValueInput->minimumValue(0);
			distanceValueInput->isMinimumValueInclusive(true);
			distanceValueInput->maximumValue(10);
			distanceValueInput->isMaximumValueInclusive(true);

			// Create distance value input 2.
			Ptr<DistanceValueCommandInput>  distanceValueInput2 = tab2ChildInputs->addDistanceValueCommandInput("distanceValue2", "DistanceValue 2", ValueInput::createByReal(1));
			if (!distanceValueInput2)
				return;
			distanceValueInput2->setManipulator(Point3D::create(0, 0, 0), Vector3D::create(0, 1, 0));
			distanceValueInput2->expression("1 in");
			distanceValueInput2->hasMinimumValue(false);
			distanceValueInput2->hasMaximumValue(false);

			// Create table input.
			Ptr<TableCommandInput> tableInput = tab2ChildInputs->addTableCommandInput("table", "Table", 3, "1:1:1");
			addRowToTable(tableInput);

			// Add inputs into the table.
			Ptr<CommandInput> addButtonInput = tab2ChildInputs->addBoolValueInput("tableAdd", "Add", false, "", true);
			tableInput->addToolbarCommandInput(addButtonInput);
			Ptr<CommandInput> deleteButtonInput = tab2ChildInputs->addBoolValueInput("tableDelete", "Delete", false, "", true);
			tableInput->addToolbarCommandInput(deleteButtonInput);

			// Create angle value input.
			Ptr<AngleValueCommandInput> angleValueInput = tab2ChildInputs->addAngleValueCommandInput("angleValue", "AngleValue", ValueInput::createByString("30 degree"));
			angleValueInput->setManipulator(Point3D::create(0, 0, 0), Vector3D::create(1, 0, 0), Vector3D::create(0, 0, 1));
			angleValueInput->hasMinimumValue(false);
			angleValueInput->hasMaximumValue(false);
		}
	}
}

SMCommandCreatedEventHandler theCmdCreatedHandler;