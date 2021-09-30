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


#ifndef __AUDIOMONITOR_H__
#define __AUDIOMONITOR_H__


#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../GenericProcessor/GenericProcessor.h"
#include "../Dsp/Dsp.h"

/**
  Reads data from a file.

  @see GenericProcessor
*/
class AudioMonitor : public GenericProcessor
{
public:
    AudioMonitor();
    ~AudioMonitor();

    void process (AudioSampleBuffer& buffer) override;
    void setParameter (int parameterIndex, float newValue) override;

    AudioProcessorEditor* createEditor() override;

    bool hasEditor() const  override { return true; }

    void updateSettings() override;

    void setChannelStatus(int chan, bool status);

    /** Updates the audio buffer size*/
	void updatePlaybackBuffer();

    void prepareToPlay(double sampleRate_, int estimatedSamplesPerBlock) override;

    bool startAcquisition() override;

    void updateFilter(int i);

    std::vector<bool> dataChannelStates;

private:
    void recreateBuffers();

    std::map<int, std::unique_ptr<AudioSampleBuffer>> bufferA;
    std::map<int, std::unique_ptr<AudioSampleBuffer>> bufferB;

    std::map<int, int> numSamplesExpected;

    std::map<int, int> samplesInBackupBuffer;
    std::map<int, int> samplesInOverflowBuffer;
    std::map<int, double> sourceBufferSampleRate;
    double destBufferSampleRate;
	int estimatedSamples;

    bool isMuted;

    enum AudioOutputType {LEFT = 1, BOTH, RIGHT};

    AudioOutputType audioOutput; 

    std::map<int, bool> bufferSwap;

    // sample rate, timebase, and ratio info:
    std::map<int, double> ratio;

    // major objects:
    std::map<int, std::unique_ptr<Dsp::Filter>> filters;

    std::unique_ptr<AudioSampleBuffer> tempBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioMonitor);
};


#endif  // __AUDIOMONITOR_H__
