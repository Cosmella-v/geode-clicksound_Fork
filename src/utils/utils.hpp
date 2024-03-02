#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

Mod* newIndexToMod(std::string url) {
    auto& metadata = ModMetadata::create(matjson::parse("{}")).unwrap();
    web::AsyncWebRequest()
        .fetch(url)
	    .json()
        .then([&](auto const& webRes){
            auto res = webRes["payload"];
            metadata->setVersion(res["versions"][0]["name"]);
            metadata->setName(res["versions"][0]["version"]);
            metadata->setDevelopers({res["developers"][0]["display_name"]});
            metadata->setDescription(res["versions"][0]["description"]);
	    });
    Mod theNodeIds = Mod(metadata);
    Mod* theNodeId2 = &mtheNodeIdsod;
    return theNodeId2;
}