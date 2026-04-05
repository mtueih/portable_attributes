set_project("portable_attributes")
set_version("1.0.0")

add_rules("mode.release", "mode.debug")

target("portable_attributes")
    set_kind("headeronly")
    add_includedirs("include", {public = true})