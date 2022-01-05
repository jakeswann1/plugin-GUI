/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

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

#ifndef __SPIKEDETECTORCONFIGWINDOW_H_F0BD2DD9__
#define __SPIKEDETECTORCONFIGWINDOW_H_F0BD2DD9__


#include <EditorHeaders.h>

#include "SpikeDetector.h"

class SpikeDetectorEditor;
class PopupConfigurationWindow;
class SpikeDetectorTableModel;

//class SpikeDetectorTableHeader : public TableHeaderComponent
//{
 //   SpikeDetector
//}

/** 
*   Table component used to edit Spike Channel names
*/
class EditableTextCustomComponent : 
    public juce::Label,
    public Label::Listener
{
public:

    /** Constructor */
    EditableTextCustomComponent(StringParameter* name_, bool acquisitionIsActive_)
        : name(name_),
          acquisitionIsActive(acquisitionIsActive_)
    {
        setEditable(false, !acquisitionIsActive, false);
        addListener(this);
        setColour(Label::textColourId, Colours::white);
        setColour(Label::textWhenEditingColourId, Colours::yellow);
        setColour(TextEditor::highlightedTextColourId, Colours::yellow);
    }

    void mouseDown(const juce::MouseEvent& event) override;
    
    void labelTextChanged(Label* label);

    void setRowAndColumn(const int newRow, const int newColumn);
    
    void setParameter(StringParameter* name_) { name = name_; }

    int row;

private:
    StringParameter* name;
    bool acquisitionIsActive;
    int columnId;
};

/**
*   Table component used to edit the continuous channels
*   used by a Spike Channel
*/
class ChannelSelectorCustomComponent : public juce::Label,
public PopupChannelSelector::Listener
{
public:
    ChannelSelectorCustomComponent(SelectedChannelsParameter* channels_, bool acquisitionIsActive_)
        : channels(channels_),
          acquisitionIsActive(acquisitionIsActive_)
    {
        setEditable(false, false, false);
    }

    void mouseDown(const juce::MouseEvent& event) override;
    
    void channelStateChanged(Array<int> newChannels) override
    {
        Array<var> newArray;
    
        for (int i = 0; i < newChannels.size(); i++)
        {
            newArray.add(newChannels[i]);
            std::cout << "Channel " << newChannels[i] << " selected" << std::endl;
        }
        
        String s = "[";
        
        for (auto chan : newArray)
        {
            s += String(int(chan)+1) + ",";
        }
        
        s += "]";
        
        setText(s, dontSendNotification);
            
        channels->setNextValue(newArray);
    
    }
    
    void setRowAndColumn(const int newRow, const int newColumn);
    
    void setParameter(SelectedChannelsParameter* channels_) { channels = channels_; }

    int row;

private:
    SelectedChannelsParameter* channels;
    int columnId;
    juce::Colour textColour;
    bool acquisitionIsActive;
};

class ThresholdSelectorCustomComponent;

class PopupThresholdComponent : public Component,
    public Slider::Listener,
    public Button::Listener
{
public:
    PopupThresholdComponent(SpikeDetectorTableModel* table,
                            ThresholdSelectorCustomComponent* owner,
                            int row,
                            int numChannels,
                            ThresholderType type,
                            Array<FloatParameter*> abs_thresholds,
                            Array<FloatParameter*> dyn_thresholds,
                            Array<FloatParameter*> std_thresholds,
                            bool isLocked);
    ~PopupThresholdComponent();
    
    void createSliders();
    
    void sliderValueChanged(Slider* slider);
    void buttonClicked(Button* button);
    
private:
    std::unique_ptr<UtilityButton> lockButton;
    std::unique_ptr<UtilityButton> absButton;
    std::unique_ptr<UtilityButton> stdButton;
    std::unique_ptr<UtilityButton> dynButton;
    std::unique_ptr<Label> label;
    OwnedArray<Slider> sliders;
    
    Array<FloatParameter*> abs_thresholds;
    Array<FloatParameter*> dyn_thresholds;
    Array<FloatParameter*> std_thresholds;
    
    ThresholderType thresholdType;
    SpikeDetectorTableModel* table;
    ThresholdSelectorCustomComponent* owner;
    
    const int sliderWidth = 18;
    int row;
};

/**
*   Table component used to edit the thresholds
*   used by a Spike Channel
*/
class ThresholdSelectorCustomComponent : public Component
{
public:
    ThresholdSelectorCustomComponent(SpikeChannel* channel_, bool acquisitionIsActive_);
    ~ThresholdSelectorCustomComponent();
    
    void setSpikeChannel(SpikeChannel* channel);

    void mouseDown(const juce::MouseEvent& event) override;
    
    void paint(Graphics& g) override;

    void setRowAndColumn(const int newRow, const int newColumn);

    void setThreshold(ThresholderType type, int channelNum, float value);
    
    void setTableModel(SpikeDetectorTableModel* table_) { table = table_; };

    int row;
    
    SpikeChannel* channel;

private:

    SpikeDetectorTableModel* table;
    

    int columnId;
    juce::Colour textColour;
    bool acquisitionIsActive;
    
    int numChannels;
    
    Array<FloatParameter*> dyn_thresholds;
    Array<FloatParameter*> abs_thresholds;
    Array<FloatParameter*> std_thresholds;
    
    CategoricalParameter* thresholder_type;
    
    String thresholdString;
    

};

/**
*   Table component used to select the waveform type
*   (full vs. peak) for a Spike Channel.
*/
class WaveformSelectorCustomComponent : public Component
{
public:
    WaveformSelectorCustomComponent(CategoricalParameter* waveformtype_, bool acquisitionIsActive_)
        : waveformtype(waveformtype_),
          acquisitionIsActive(acquisitionIsActive_)
    {
    }

    void mouseDown(const juce::MouseEvent& event) override;
    
    void paint(Graphics& g) override;
    
    void setRowAndColumn(const int newRow, const int newColumn);
    
    void setParameter(CategoricalParameter* waveformtype_) { waveformtype = waveformtype_; }

    void setWaveformValue(int value);

    void setTableModel(SpikeDetectorTableModel* table_) { table = table_; };

    int row;

private:
    CategoricalParameter* waveformtype;
    SpikeDetectorTableModel* table;
    int columnId;
    juce::Colour textColour;
    bool acquisitionIsActive;
};

/**
*   TableListBoxModel used for editing Spike Channel parameters
*/
class SpikeDetectorTableModel : public TableListBoxModel
{

public:

    SpikeDetectorTableModel(SpikeDetectorEditor* editor, 
                            PopupConfigurationWindow* owner,
                            bool acquisitionIsActive);

    enum Columns {
        INDEX = 1,
        NAME,
        TYPE,
        CHANNELS,
        THRESHOLD,
        WAVEFORM,
        DELETE
    };

    void cellClicked(int rowNumber, int columnId, const MouseEvent& event) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
        Component* existingComponentToUpdate) override;

    int getNumRows() override;
    
    void update(Array<SpikeChannel*> spikeChannels);

    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    void broadcastWaveformTypeToSelectedRows(int rowThatWasClicked, int value);
    void broadcastThresholdToSelectedRows(int rowThatWasClicked, ThresholderType type, int channelIndex, bool isLocked, float value);
    void broadcastThresholdTypeToSelectedRows(int rowThatWasClicked, ThresholderType type);

    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Array<SpikeChannel*> spikeChannels;
    TableListBox* table;
private:

    OwnedArray<WaveformSelectorCustomComponent> waveformComponents;
    OwnedArray<ThresholdSelectorCustomComponent> thresholdComponents;

    SpikeDetectorEditor* editor;
    
    PopupConfigurationWindow* owner;

    bool acquisitionIsActive;

};

/**
*   Popup window used to edit Spike Channel settings
*/
class PopupConfigurationWindow : public Component
{

public:
    
    /** Constructor */
    PopupConfigurationWindow(SpikeDetectorEditor* editor, 
                             Array<SpikeChannel*> spikeChannels,
                             bool acquisitionIsActive);

    /** Destructor */
    ~PopupConfigurationWindow() { }

    /** Updates the window with a new set of Spike Channels*/
    void update(Array<SpikeChannel*> spikeChannels);

    /** Viewport to allow scrolling of the table*/
    std::unique_ptr<Viewport> tableViewport;

    /** Custom table header component (not currently used)*/
    //std::unique_ptr<TableHeaderComponent> tableHeader;

    /** Custom table model*/
    std::unique_ptr<SpikeDetectorTableModel> tableModel;

    /** Custom list box for Spike Channel settings*/
    std::unique_ptr<TableListBox> electrodeTable;

private:
    SpikeDetectorEditor* editor;
};


#endif  // __SPIKEDETECTORCONFIGWINDOW_H_F0BD2DD9__
