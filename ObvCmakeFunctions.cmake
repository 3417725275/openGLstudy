cmake_minimum_required (VERSION 3.13)

#初始化宏，include后必须调用该宏对本页方法中的变量进行初始化，projectCMakePath为项目路径，is64bit为程序类型是否为64位
macro(init_obv_cmake projectCMakePath is64bit)
#######################global variable#######################
set(THIRD_PARTY "${projectCMakePath}/../3rdParty")
set(IS64BIT ${is64bit})
#set lib suffix
if(MSVC)
	set(PLATFORM "win")
	set(DLL_PLATFORM_NAME "dll")
elseif (UNIX)
	set(PLATFORM "linux")
	set(DLL_PLATFORM_NAME "so")
else()
	message(FATAL_ERROR "unkonwn palform")
endif()
#set excuble bit
if(IS64BIT)
set(PLATFORM "${PLATFORM}64")
else()
set(PLATFORM "${PLATFORM}32")
endif()
#set(CMAKE_DEBUG_POSTFIX "d")
if(NOT DEFINED BIN_OUTPUT_PATH)
message("default BIN_OUTPUT_PATH, using -DBIN_OUTPUT_PATH=xxx command to define it.")
set(BIN_OUTPUT_PATH "${projectCMakePath}/build/${PLATFORM}/${CMAKE_BUILD_TYPE}")
endif()
set(EXECUTABLE_OUTPUT_PATH ${BIN_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${BIN_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH})
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(BIN_INSTALL_NAME "Debug")
else()
	set(BIN_INSTALL_NAME "Release")
endif()

message("THIRD_PARTY:${THIRD_PARTY}")
message("PLATFORM:${PLATFORM}")
message("BIN_OUTPUT_PATH:${BIN_OUTPUT_PATH}")
message("CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message("BIN_INSTALL_NAME:${BIN_INSTALL_NAME}")
message("PROJECT_BINARY_DIR:${PROJECT_BINARY_DIR}")
endmacro()
#添加链接选项，参数值为任意个，空格隔开。
macro(mac_add_link_options)
    foreach(OPTION ${ARGV})
        set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${OPTION}")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OPTION}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OPTION}")

		set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${OPTION}")
        set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${OPTION}")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${OPTION}")

		set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO    "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${OPTION}")
        set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO} ${OPTION}")
        set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} ${OPTION}")
    endforeach()
endmacro()
#修改所有的编译选项，将MD改为MT。
macro(mac_md_to_mt_in_msvc)
	# Use the static C library for all build types
	foreach(var 
		CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
		)
	if(${var} MATCHES "/MD")
		string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
	endif()
	endforeach()
endmacro()
#######################global variable#######################
#不通过cmake预设构建时，通过命令行构建时该方法会将源代码文件按照文件夹对项目进行组织，<目标名称><源代码文件集合><生成类型>。生成类型为STATIC/SHARED/EXE，分别对应动态库/静态库/可执行文件。
function(func_source_group_by_dir_in_msvc exe_name all_files type_string)
	set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
	foreach(sgbd_file ${all_files})
		string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
		string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
		string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
		string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
		string(REPLACE "." "\\" sgbd_group_name ${sgbd_group_name})
		if(sgbd_nogroup)
			set(sgbd_group_name "\\")
		endif(sgbd_nogroup)
		source_group(${sgbd_group_name} FILES ${sgbd_file})
	endforeach(sgbd_file)
endfunction(func_source_group_by_dir_in_msvc)
#添加执行文件到项目，<目标名称><源代码文件集合><生成类型>。生成类型为STATIC/SHARED/EXE，分别对应静态库/动态库/可执行文件。
function(func_add_executable executable_name code_source type_string)
	if("${type_string}" STREQUAL "STATIC")
		add_library(${executable_name} STATIC ${code_source})#DLL OR LIB
	elseif("${type_string}" STREQUAL "SHARED")
		add_library(${executable_name} SHARED ${code_source})#DLL OR LIB
	elseif("${type_string}" STREQUAL "EXE")
		add_executable(${executable_name} ${code_source})# EXE OR O
	else()
		message(FATAL_ERROR "unkonwn type executable:${type_string},must be SHARED OR STATIC OR EXE")
	endif()
	target_sources(${executable_name} PRIVATE ${code_source})
endfunction(func_add_executable)
#内部函数-添加依赖库
function(_func_glob_add executable_name libpath type_string)
	file(GLOB_RECURSE lib "${libpath}")
	foreach(tmpvar ${lib})
		get_filename_component(file_name ${tmpvar} NAME)
		list(FIND ARGN ${file_name} RET)
		if (${RET} EQUAL -1)
			target_link_libraries(${executable_name} PUBLIC ${type_string} ${tmpvar})
		endif()
	endforeach()
endfunction()
#内部函数-拷贝文件
function(_func_glob_install executable_name libpath)
	file(GLOB_RECURSE lib "${libpath}")
	foreach(tmpvar ${lib})
		get_filename_component(file_name ${tmpvar} NAME)
		list(FIND ARGN ${file_name} RET)
		if (${RET} EQUAL -1)
			file(INSTALL ${tmpvar} DESTINATION ${BIN_OUTPUT_PATH} FOLLOW_SYMLINK_CHAIN)
		endif()
	endforeach()
endfunction()
#内部函数-添加第三方库WIN
function(_func_add_inc_libs_msvc executable_name lib_relative_path lib_type_string)
		_func_glob_add(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.lib" ${lib_type_string} "${ARGN}")
		_func_glob_install(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.dll" "${ARGN}")
		_func_glob_install(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.pdb" "${ARGN}")
endfunction()
#内部函数-添加第三方库UNIX
function(_func_add_inc_libs_unix executable_name lib_relative_path lib_type_string)
		_func_glob_add(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.a" ${lib_type_string} "${ARGN}")
		_func_glob_add(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.so" ${lib_type_string} "${ARGN}")
		_func_glob_install(${executable_name} "${THIRD_PARTY}/${lib_relative_path}/*.so" "${ARGN}")
endfunction()
#添加第三方库到当前项目，该方法会自动拷贝目录的运行依赖文件到生成目录${BIN_OUTPUT_PATH}。<头文件路径><Debug版本库路径><Release版本库路径>[排除项，分号隔开]。
function(func_add_3rdparty_inc_libs executable_name include_dir debug_lib_relative_path optimized_lib_relative_path)
	target_include_directories(${executable_name} PUBLIC "${THIRD_PARTY}/${include_dir}")
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(make_type "debug")
		set(find_dir ${debug_lib_relative_path})
	else()
		set(make_type "optimized")
		set(find_dir ${optimized_lib_relative_path})
	endif()
	if (MSVC)
		if(NOT DEFINED CMAKE_BUILD_TYPE)
			_func_add_inc_libs_msvc(${executable_name} ${debug_lib_relative_path} "debug" "${ARGN}")
			_func_add_inc_libs_msvc(${executable_name} ${optimized_lib_relative_path} "optimized" "${ARGN}")
		else()
			_func_add_inc_libs_msvc(${executable_name} ${find_dir} ${make_type} "${ARGN}")
		endif()
	elseif (UNIX)
		if(NOT DEFINED CMAKE_BUILD_TYPE)
			_func_add_inc_libs_unix(${executable_name} ${debug_lib_relative_path} "debug" "${ARGN}")
			_func_add_inc_libs_unix(${executable_name} ${optimized_lib_relative_path} "optimized" "${ARGN}")
		else()
			_func_add_inc_libs_unix(${executable_name} ${find_dir} ${make_type} "${ARGN}")
		endif()
	else()
		message(FATAL_ERROR "unkonwn platform: not msvc and unix.")
	endif()
endfunction()
#生成后动作，将指定的文件夹下的文件拷贝指定目录，不遵循FOLLOW_SYMLINK_CHAIN。<目标名称><源文件夹><目的文件夹>。
function(func_copy_folder_not_followlinks_after_build target_name src_path des_path)
if(MSVC)
	add_custom_command(TARGET ${target_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${src_path}" "${des_path}"
	)
elseif(UNIX)
set(target_zip_name "${des_path}/${target_name}_temp.zip")
	add_custom_command(TARGET ${target_name} POST_BUILD
		WORKING_DIRECTORY ${src_path}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${des_path}
		COMMAND ${CMAKE_COMMAND} -E tar cfv "${target_zip_name}" "."
	)
	add_custom_command(TARGET ${target_name} POST_BUILD
		WORKING_DIRECTORY ${des_path}
		COMMAND ${CMAKE_COMMAND} -E tar xfv "${target_zip_name}"
		COMMAND ${CMAKE_COMMAND} -E remove "${target_zip_name}"
	)
else()
	message(FATAL_ERROR "unkonwn platform: not msvc and unix.")
endif()
endfunction()
#拷贝文件夹，将指定文件夹（不是文件）复制到对应目录，包含该文件夹，支持通过通配符筛选文件，筛选时拷贝的是文件（不是文件夹）。<来源文件夹><拷贝位置>
function(func_copy_folder src_path des_path)
	file(GLOB all_files "${src_path}")
	file(COPY ${all_files} DESTINATION "${des_path}")
endfunction()

#从RC模板创建一个填充当前程序信息的实例。<输出的RC文件保存位置><目标名称><程序版本>。
function(func_generate_rc_file_in_msvc output_rc_file_path e_name e_version)
	set(EXECUTABLE_VERSION ${e_version})
	set(EXECUTABLE_NAME ${e_name})
	set(VersionInfoTemplate "${THIRD_PARTY}/cmake/VersionInfoTemplate.rc")
	configure_file(${VersionInfoTemplate} ${output_rc_file_path})
endfunction()

#添加源代码目录到可执行文件。<目标名称><代码目录集合><生成类型><程序版本>。生成类型为STATIC/SHARED/EXE，分别对应静态库/动态库/可执行文件；版本为空字符时不添加版本信息。
function(func_add_code_path_to_executable exe_name code_paths type_string execuable_version)
	set(all_files "")
	foreach(tmpvar ${code_paths})
		file(GLOB_RECURSE getted_files FOLLOW_SYMLINKS FALSE "${tmpvar}/*.h" "${tmpvar}/*.hpp" "${tmpvar}/*.cpp" "${tmpvar}/*.cxx" "${tmpvar}/*.cc" "${tmpvar}/*.c" "${tmpvar}/*.inl")
		list(APPEND all_files "${getted_files}")
	endforeach()
	if(MSVC) #add code source msvc
		message("func_add_code_path_to_executable:msvc platform")
		#add version
		if(NOT("${execuable_version}" STREQUAL ""))
			set(MY_VERSIONINFO_RC "${PROJECT_BINARY_DIR}/VersionInfo.rc")
			func_generate_rc_file_in_msvc("${MY_VERSIONINFO_RC}" ${exe_name} ${execuable_version})
			set(all_files ${all_files} ${MY_VERSIONINFO_RC})
		endif()
		func_add_executable(${exe_name} "${all_files}" "${type_string}")
		func_source_group_by_dir_in_msvc(${exe_name} "${all_files}" "${type_string}")
	else()	#add code source not msvc
		message("func_add_code_path_to_executable:not msvc platform")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath=./")
		func_add_executable(${exe_name} "${all_files}" "${type_string}")
		#add version
		if(NOT("${execuable_version}" STREQUAL ""))
			set_target_properties(${exe_name} PROPERTIES VERSION "${execuable_version}")
		endif()
	endif()
	add_definitions(-DUNICODE -D_UNICODE)
endfunction()
###########################function end###########################################
