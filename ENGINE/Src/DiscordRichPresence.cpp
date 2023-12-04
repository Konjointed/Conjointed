#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>

#include "DiscordRichPresence.h"

discord::Core* core{};

DiscordRichPresence::DiscordRichPresence() {
	// Load our .env file so that we can get the client id
	std::map<std::string, std::string> envMap;
	std::ifstream file("../.env");
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '=')) {
			std::string value;
			if (std::getline(is_line, value)) {
				envMap[key] = value;
			}
		}
	}

	auto it = envMap.find("DISCORD_CLIENT_ID");
	if (it != envMap.end()) {
		discord::Result result = discord::Core::Create(std::stoll(it->second), DiscordCreateFlags_Default, &core);
		if (result != discord::Result::Ok) {
			std::cerr << "Failed to initialize Discord Core" << std::endl;
			exit(-1);
		}
	}
	else {
		std::cout << "DISCORD_CLIENT_ID not found in .env\n";
		return;
	}

	int64_t startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	SetDiscordPresence("Editing Untitled Project", "", startTimestamp);
}

DiscordRichPresence::~DiscordRichPresence() {}

void DiscordRichPresence::Update() {
	core->RunCallbacks();
}

void DiscordRichPresence::SetDiscordPresence(const std::string& state, const std::string& details, int64_t startTimestamp) {
	discord::Activity activity{};
	activity.SetState(state.c_str());
	//activity.SetDetails(details.c_str());
	activity.GetTimestamps().SetStart(startTimestamp);

	activity.GetAssets().SetLargeImage("logo_red");
	//activity.GetAssets().SetSmallImage("small_image_key");

	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
		if (result != discord::Result::Ok) {
			std::cerr << "Failed to update activity" << std::endl;
		}
		});
}