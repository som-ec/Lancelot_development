/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/properties.h>
#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "product.",
    "system.",
    "vendor.",
    "system_ext.",
};

void property_override(char const prop[], char const value[], bool add = true);

void property_override(char const prop[], char const value[], bool add)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}


void set_ro_build_prop(const std::string &source, const std::string &prop,
        const std::string &value, bool product = false) {
    std::string prop_name;

    if (product) {
        prop_name = "ro.product." + source + prop;
    } else {
        prop_name = "ro." + source + "build." + prop;
    }

    property_override(prop_name.c_str(), value.c_str(), false);
}

void set_device_props(const std::string fingerprint, const std::string description,
        const std::string brand, const std::string device, const std::string model) {
    for (const auto &source : ro_props_default_source_order) {
        set_ro_build_prop(source, "fingerprint", fingerprint);
        set_ro_build_prop(source, "brand", brand, true);
        set_ro_build_prop(source, "device", device, true);
        set_ro_build_prop(source, "model", model, true);
    }

    property_override("ro.build.fingerprint", fingerprint.c_str());
    property_override("ro.build.description", description.c_str());
}

void load_device_properties() {
    std::string hwname = GetProperty("ro.product.vendor.device", "");
    std::string region = GetProperty("ro.boot.hwc", "");

    if (hwname == "lancelot") {
        set_device_props(
                "google/redfin/redfin:11/RQ1A.210205.004/7038034:user/release-keys",
                "redfin-user 11 RQ1A.210205.004 7038034 release-keys",
                "Redmi", "lancelot", "M2004J19C");
    } else if (hwname == "galahad") {
        set_device_props(
                "google/redfin/redfin:11/RQ1A.210205.004/7038034:user/release-keys",
                "redfin-user 11 RQ1A.210205.004 7038034 release-keys",
                "Redmi", "galahad", "M2004J19C");
    } else if (hwname == "shiva") {
        set_device_props(
                "google/redfin/redfin:11/RQ1A.210205.004/7038034:user/release-keys",
                "redfin-user 11 RQ1A.210205.004 7038034 release-keys",
                "POCO", "shiva", "M2004J19PI"); }
}

void vendor_load_properties() {
    load_device_properties();
}