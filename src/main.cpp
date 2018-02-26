/* game
 * Copyright (C) 2014-2018 Scott Bishop <treewojima@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "defines.h"

#include <boost/filesystem/path.hpp>
#ifndef _NO_PROGRAM_OPTIONS
#   include <boost/program_options.hpp>
#endif
#include <iostream>

#include "game.h"
#include "logger.h"

// Locals
namespace
{
    const unsigned DEFAULT_WINDOW_WIDTH  = 640;
    const unsigned DEFAULT_WINDOW_HEIGHT = 480;

    Game::Options parseArgs(int argc, char *argv[]);

    Game *_game;
}

Game *getGame()
{
    return _game;
}

int main(int argc, char *argv[])
{
    try
    {
        auto options = parseArgs(argc, argv);

        _game = new Game(options);
        _game->run();
    }
    catch (std::exception &e)
    {
        LOG_ERROR << "EXCEPTION: " << e.what();
    }

    delete _game;
    LOG_INFO << "shut down cleanly";
    return 0;
}

// Local functions
namespace {

Game::Options parseArgs(int argc, char *argv[])
{
#ifndef _NO_PROGRAM_OPTIONS
    namespace fs = boost::filesystem;
    namespace po = boost::program_options;

    Game::Options options;

    fs::path programPath(argv[0]);
    options.programName = programPath.filename().replace_extension().string();

    std::ostringstream defaultLogFile;
    defaultLogFile << "logs/" << options.programName << ".txt";

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("width,w", po::value<unsigned>(&options.windowWidth)->default_value(DEFAULT_WINDOW_WIDTH), "set window width")
        ("height,h", po::value<unsigned>(&options.windowHeight)->default_value(DEFAULT_WINDOW_HEIGHT), "set window height")
        ("log-file", po::value<std::string>(&options.logFile)->default_value(defaultLogFile.str()), "set output log file")
        ("suppress-ogre-log,q", po::bool_switch(&options.suppressOgreLog)->default_value(false), "suppress OGRE log output");

    po::variables_map map;
    po::store(po::parse_command_line(argc, argv, desc), map);
    po::notify(map);

    if (map.count("help"))
    {
        std::cout << desc << std::endl;
        std::exit(0);
    }

#else
#warning "building w/o program options"
    Game::Options options;
    options.logFile = "logs/tile.txt";
    options.programName = "tile";
    options.windowHeight = DEFAULT_WINDOW_HEIGHT;
    options.windowWidth = DEFAULT_WINDOW_WIDTH;
    options.suppressOgreLog = false;
#endif

    return options;
}

}
