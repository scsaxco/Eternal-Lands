/****************************************************************************
 *            logging.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "logging.hpp"
#include <bitset>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <SDL/SDL_mutex.h>
#include "../elc_private.h"

namespace eternal_lands
{

	namespace
	{

		SDL_mutex* log_mutex;
		std::ofstream log_file;
		LogLevelType log_levels = llt_info;

		void log_message(const std::string &type,
			const std::string &message, const std::string &file,
			const Uint32 line)
		{
			char buffer[128];
			std::time_t raw_time;

			if (!log_file.is_open())
			{
				return;
			}

			std::time(&raw_time);
			memset(buffer, 0, sizeof(buffer));
			std::strftime(buffer, sizeof(buffer), "%X",
				std::localtime(&raw_time));

			log_file << "[" << buffer;

			if (log_levels >= llt_debug_verbose)
			{
				log_file << ", " << file << ":" << line;
			}

			log_file << "] " << type << ": " << message;

			if (message.find('\n') == std::string::npos)
			{
				log_file << "\n";
			}
		}

		std::string get_str(const LogLevelType log_level)
		{
			switch (log_level)
			{
				case llt_error:
					return "Error";
				case llt_warning:
					return "Warning";
				case llt_info:
					return "Info";
				case llt_debug:
				case llt_debug_verbose:
					return "Debug";
				default:
					return "Unkown";
			}
		}

		void rename_old_logfile(const std::string &log_file_name,
			const Uint32 index)
		{
			std::stringstream str1, str2;

			str1 << log_file_name << "." << index;
			str2 << log_file_name << "." << (index + 1);

			std::rename(str1.str().c_str(), str2.str().c_str());
		}

		std::string get_local_time_string()
		{
			char buffer[512];
			std::time_t raw_time;

			std::time(&raw_time);

			memset(buffer, 0, sizeof(buffer));
			std::strftime(buffer, sizeof(buffer), "%c %Z",
				std::localtime(&raw_time));

			return buffer;
		}

	}

	void init_logging(const std::string &log_file_name)
	{
		std::string str;

		log_mutex = SDL_CreateMutex();

		rename_old_logfile(log_file_name, 8);
		rename_old_logfile(log_file_name, 7);
		rename_old_logfile(log_file_name, 6);
		rename_old_logfile(log_file_name, 5);
		rename_old_logfile(log_file_name, 4);
		rename_old_logfile(log_file_name, 3);
		rename_old_logfile(log_file_name, 2);
		rename_old_logfile(log_file_name, 1);
		rename_old_logfile(log_file_name, 0);

		str = log_file_name;
		str += ".0";
		std::rename(log_file_name.c_str(), str.c_str());

		log_file.open(log_file_name.c_str(),
			std::ios::out | std::ios::binary);

		log_message("Log started at", get_local_time_string(),
			__FILE__, __LINE__);

		log_message("version", FILE_VERSION, __FILE__, __LINE__);

		if (log_file.is_open())
		{
			log_file.flush();
		}
	}

	void exit_logging()
	{
		log_file.close();

		SDL_DestroyMutex(log_mutex);
	}

	LogLevelType get_log_level()
	{
		LogLevelType result;

		SDL_LockMutex(log_mutex);

		result = log_levels;

		SDL_UnlockMutex(log_mutex);

		return result;
	}

	void set_log_level(const LogLevelType log_level)
	{
		SDL_LockMutex(log_mutex);

		log_levels = log_level;

		SDL_UnlockMutex(log_mutex);
	}

	void log_message(const LogLevelType log_level,
		const std::string &message, const std::string &file,
		const Uint32 line)
	{
		SDL_LockMutex(log_mutex);

		if (log_levels >= log_level)
		{
			log_message(get_str(log_level), message, file, line);

			if (((log_level == llt_error) ||
				(log_levels >= llt_debug_verbose)) &&
				log_file.is_open())
			{
				log_file.flush();
			}
		}

		SDL_UnlockMutex(log_mutex);
	}

}