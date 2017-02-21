# - Uninstall target template

		if (NOT EXISTS "/home/karthik/Development/xbeeplus/build/install_manifest.txt")
			message(FATAL_ERROR "Cannot find install manifest: \"/home/karthik/Development/xbeeplus/build/install_manifest.txt\"")
		endif(NOT EXISTS "/home/karthik/Development/xbeeplus/build/install_manifest.txt")

		file(READ "/home/karthik/Development/xbeeplus/build/install_manifest.txt" files)
		string(REGEX REPLACE "\n" ";" files "${files}")

		foreach (file ${files})
			message(status "Uninstalling \"$ENV{DESTDIR}${file}\"")
			if (EXISTS "$ENV{DESTDIR}${file}")
				execute_process(
					COMMAND /usr/bin/cmake -E remove "$ENV{DESTDIR}${file}"
					OUTPUT_VARIABLE rm_out
            		RESULT_VARIABLE rm_retval
				)
				if(not ${rm_retval} equal 0)
            		message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
				endif (not ${rm_retval} equal 0)
			else (EXISTS "$ENV{DESTDIR}${file}")
				message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
			endif (EXISTS "$ENV{DESTDIR}${file}")
		endforeach(file)
	
	