/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2022 Open Ephys

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


#ifndef __EVENTTRANSLATOR_H_B327D3D2__
#define __EVENTTRANSLATOR_H_B327D3D2__


#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../GenericProcessor/GenericProcessor.h"
#include "../RecordNode/Synchronizer.h"

/** Holds settings for one stream's event channel*/
class EventTranslatorSettings
{
public:
    /** Constructor -- sets default values*/
    EventTranslatorSettings();

    /** Destructor*/
    ~EventTranslatorSettings() { }

    /** Creates an event for a particular stream*/
    TTLEventPtr createEvent(int64 sample_number, int line, bool state);

    int syncLine;

    EventChannel* eventChannel;
};

/**
  Translates events between data streams

  @see GenericProcessor
*/
class EventTranslator :
    public GenericProcessor
{
public:

    /** Constructor */
    EventTranslator();

    /** Destructor */
    ~EventTranslator();
    
    /** Responds to parameter value changes */
    void parameterValueChanged(Parameter* param);

    /** Add latest samples to the signal chain buffer */
    void process (AudioBuffer<float>& buffer) override;

    /** Creates the editor */
    AudioProcessorEditor* createEditor() override;
    
    /** Updates the EventTranslator settings*/
    void updateSettings() override;

    

private:
    
    /** Called whenever a new TTL event arrives*/
    void handleTTLEvent (TTLEventPtr event) override;
    
    Synchronizer synchronizer;

    StreamSettings<EventTranslatorSettings> settings;
    
    uint16 mainStream;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventTranslator);
};


#endif  // __EVENTTRANSLATOR_H_B327D3D2__
