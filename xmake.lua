add_rules("mode.debug", "mode.release")

if is_mode("debug") then 
    add_defines("DEBUG")
    set_optimize("none")
else 
    set_optimize("fastest")
end

set_languages("c11")

target("clex")
    set_kind("binary")
    add_files("src/*.c")

    add_includedirs("include")
target_end()
