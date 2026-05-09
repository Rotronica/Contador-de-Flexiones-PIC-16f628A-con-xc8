# The following variables contains the files used by the different stages of the build process.
set(ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${ContadorDeFlexiones_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(ContadorDeFlexiones_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/buzzer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/display.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/flexiones.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/fusibles.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/sensor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/timer0.c")
set_source_files_properties(${ContadorDeFlexiones_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(ContadorDeFlexiones_default_default_XC8_FILE_TYPE_link)
set(ContadorDeFlexiones_default_image_name "default-production.elf")
set(ContadorDeFlexiones_default_image_base_name "default-production")

# The output directory of the final image.
set(ContadorDeFlexiones_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/ContadorDeFlexiones/production")

# The full path to the final image.
set(ContadorDeFlexiones_default_full_path_to_image ${ContadorDeFlexiones_default_output_dir}/${ContadorDeFlexiones_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${ContadorDeFlexiones_default_output_dir}/${ContadorDeFlexiones_default_image_base_name}")
