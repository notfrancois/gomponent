/*
 *  This Source Code Form is subject to the terms of the Mozilla Public License,
 *  v. 2.0. If a copy of the MPL was not distributed with this file, You can
 *  obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  The original code is copyright (c) 2022, open.mp team and contributors.
 */

// Include the component's definition.
#include "Gomponent.hpp"
#include "Gamemode.hpp"
#include "Plugin.hpp"
#include "ActorEvents.hpp"
#include "CheckpointEvents.hpp"
#include "ClassEvents.hpp"
#include "ConsoleEvents.hpp"
#include "DialogEvents.hpp"
#include "GangZoneEvents.hpp"
#include "MenuEvents.hpp"
#include "ObjectEvents.hpp"
#include "PickupEvents.hpp"
#include "PlayerEvents.hpp"
#include "TextDrawEvents.hpp"
#include "VehicleEvents.hpp"
#include "CustomModelEvents.hpp"

// Required component methods.
StringView Gomponent::componentName() const
{
	return "Go";
}

SemanticVersion Gomponent::componentVersion() const
{
	return SemanticVersion(1, 0, 0, 0);
}

void Gomponent::onLoad(ICore* c)
{
	// Cache core, listen to player events.

	core = c;
	config = &c->getConfig();
	players = &c->getPlayers();

	gamemode_ = Gamemode::Get();
	plugin_ = Plugin::Get();
}

void Gomponent::onInit(IComponentList* components)
{
	actors = components->queryComponent<IActorsComponent>();
	checkpoints = components->queryComponent<ICheckpointsComponent>();
	classes = components->queryComponent<IClassesComponent>();
	console = components->queryComponent<IConsoleComponent>();
	dialogs = components->queryComponent<IDialogsComponent>();
	gangzones = components->queryComponent<IGangZonesComponent>();
	menus = components->queryComponent<IMenusComponent>();
	objects = components->queryComponent<IObjectsComponent>();
	pickups = components->queryComponent<IPickupsComponent>();
	textdraws = components->queryComponent<ITextDrawsComponent>();
	textlabels = components->queryComponent<ITextLabelsComponent>();
	vehicles = components->queryComponent<IVehiclesComponent>();
	models = components->queryComponent<ICustomModelsComponent>();

	if (actors)
	{
		actors->getEventDispatcher().addEventHandler(ActorEvents::Get());
	}

	if (checkpoints)
	{
		checkpoints->getEventDispatcher().addEventHandler(CheckpointEvents::Get());
	}

	if (classes)
	{
		classes->getEventDispatcher().addEventHandler(ClassEvents::Get());
	}

	if (console)
	{
		console->getEventDispatcher().addEventHandler(ConsoleEvents::Get());
	}

	if (dialogs)
	{
		dialogs->getEventDispatcher().addEventHandler(DialogEvents::Get());
	}

	if (gangzones)
	{
		gangzones->getEventDispatcher().addEventHandler(GangZoneEvents::Get());
	}

	if (menus)
	{
		menus->getEventDispatcher().addEventHandler(MenuEvents::Get());
	}

	if (objects)
	{
		objects->getEventDispatcher().addEventHandler(ObjectEvents::Get());
	}

	if (pickups)
	{
		pickups->getEventDispatcher().addEventHandler(PickupEvents::Get());
	}

	if (players)
	{
		players->getPlayerSpawnDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerConnectDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerStreamDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerTextDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerShotDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerChangeDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerDamageDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerClickDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerCheckDispatcher().addEventHandler(PlayerEvents::Get());
		players->getPlayerUpdateDispatcher().addEventHandler(PlayerEvents::Get());
	}

	if (textdraws)
	{
		textdraws->getEventDispatcher().addEventHandler(TextDrawEvents::Get());
	}

	if (vehicles)
	{
		vehicles->getEventDispatcher().addEventHandler(VehicleEvents::Get());
	}

	if (models)
	{
		models->getEventDispatcher().addEventHandler(CustomModelEvents::Get());
	}
}

bool Gomponent::loadGamemode() {
	StringView gamemodeName = config->getString("go.gamemode");
	if (gamemodeName.empty()) {
		core->logLn(LogLevel::Error, "go.gamemode config string is not set");
		return false;
	}

	try {
		gamemode_->load(gamemodeName.to_string());
		gamemode_->call<void>("onGameModeInit");
		return true;
	}
	catch (const std::runtime_error& error) {
		core->logLn(LogLevel::Error, "Failed to load gamemode: %s", error.what());
		return false;
	}
}

bool Gomponent::loadPlugin() {
	StringView pluginName = config->getString("go.plugin");
	if (pluginName.empty()) {
		core->logLn(LogLevel::Error, "go.plugin config string is not set");
		return false;
	}
	try {
		plugin_->load(pluginName.to_string());
		plugin_->call<void>("onGameModeInit");
		return true;
	}
	catch (const std::runtime_error& error) {
		core->logLn(LogLevel::Error, "Failed to load plugin: %s", error.what());
		return false;
	}
}

void Gomponent::onReady() {
	if (!loadGamemode()) {
		return;
	}
	
	if (!loadPlugin()) {
		return;
	}
}

void Gomponent::onFree(IComponent* component)
{
#define COMPONENT_UNLOADED(var) \
	if (component == var)       \
		var = nullptr;

	COMPONENT_UNLOADED(actors)
	COMPONENT_UNLOADED(checkpoints)
	COMPONENT_UNLOADED(classes)
	COMPONENT_UNLOADED(console)
	COMPONENT_UNLOADED(dialogs)
	COMPONENT_UNLOADED(gangzones)
	COMPONENT_UNLOADED(menus)
	COMPONENT_UNLOADED(objects)
	COMPONENT_UNLOADED(pickups)
	COMPONENT_UNLOADED(textdraws)
	COMPONENT_UNLOADED(textlabels)
	COMPONENT_UNLOADED(vehicles)
	COMPONENT_UNLOADED(models)
}

void Gomponent::free()
{
	// Deletes the component.
	delete this;
}

void Gomponent::reset()
{
	// Resets data when the mode changes.
}

// When this component is destroyed we need to tell any linked components this it is gone.
Gomponent::~Gomponent()
{
	// unload gamemode
	gamemode_->call<void>("onGameModeExit");
	gamemode_->unload();

	// unload plugin
	plugin_->call<void>("onGameModeExit");
	plugin_->unload();
}
