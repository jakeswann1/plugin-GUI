/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PRBFORMAT_H_330E50E0__
#define __PRBFORMAT_H_330E50E0__


#include <ProcessorHeaders.h>

#include "ChannelMappingNode.h"
#include <fstream>
#include <vector>
#include <string>

class PrbFormat
{
public:

    static void write(File filename, ChannelMapSettings* settings)
    {
        // Open the file for writing
        std::ofstream outputStream(filename.getFullPathName().toStdString());
        if (!outputStream.is_open()) {
            std::cerr << "Unable to open file for writing: " << filename.getFullPathName().toStdString() << std::endl;
            return;
        }

        // Write CSV header
        outputStream << "mapping,enabled" << std::endl;

        // Write channel data
        for (int i = 0; i < settings->channelOrder.size(); i++)
        {
            outputStream << settings->channelOrder[i] << ","
                         << settings->isEnabled[i] << std::endl;
        }

        // Close the file
        outputStream.close();
    }

    static void read(File filename, ChannelMapSettings* settings)
    {
        FileInputStream inputStream(filename);

        var json = JSON::parse(inputStream);

        var returnVal = -255;

        var channelGroup = json.getProperty(Identifier("0"), returnVal);

        if (channelGroup.equalsWithSameType(returnVal))
        {
            return;
        }

        var mapping = channelGroup[Identifier("mapping")];
        Array<var>* map = mapping.getArray();

        var enabled = channelGroup[Identifier("enabled")];
        Array<var>* enbl = enabled.getArray();

        for (int i = 0; i < map->size(); i++)
        {
            int ch = map->getUnchecked(i);
            settings->channelOrder.set(i, ch);

            bool en = enbl->getUnchecked(i);
            settings->isEnabled.set(i, en);
        }
    }
};


#endif  // __PRBFORMAT_H_330E50E0__
