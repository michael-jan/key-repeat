# Key Repeat Manual

Welcome to Key Repeat's manual! It is currently a work in progress.

![](docs/images/plugin-screenshot.png)

## Official Website
For downloads and more info, check out our [official site](https://michael-jan.github.io/key-repeat/ "Key Repeat's official website")! Key Repeat is available in VST3 and AU formats on both Windows and Mac.

## Quickstart Guide
1. Download the plugin from our [official site](https://michael-jan.github.io/key-repeat/ "Key Repeat's official website") and install it using the included instructions.
2. Launch Key Repeat from your DAW.
3. Drag and drop an audio sample from your file explorer.
4. Hold middle C together with one of the keyswitches, and you'll hear your sample being played repeatedly.
5. Start recording hi-hat patterns and snare rolls in real time!

## Introduction

Key Repeat is an open source beat repeat sampler. Use it to record note repeats in real time!

The dark box on the right is where you should drag in the audio sample you want to record repeats for. 

By default, C0 thru F#0 are the designated "keyswitches" which you'll use to adjust how fast your sample will repeat itself. All other keys on your MIDI keyboard (henceforce called "playback keys") can be used to play your sample.

To play or record note repeats, hold down both a keyswitch and a playback key simultaneously. The plugin will sync its tempo with your DAW.

## Controls
Key Repeat provides a complete set of controls that lets you customize your sound and workflow.

### Modes
<b>Latch</b>: With latch mode turned off, you won't hear note repeats unless you are currently holding both a keyswitch and a playback key. With latch mode turned on, the keyswitches become "sticky." So, if you let go of a keyswitch, note repeats will continue playing as long as you're still holding down a playback key. 
<br>

<b>EZ Triplet</b>: With EZ Triplet off, you'll have to hold two keyswitches down to play triplets, one corresponding to the repeat rate and the one labelled "Trip." With EZ Triplet on, the plugin will remap your keyswitches so you'll only have to hold one keyswitch at a time, even for triplets.

### Sound Controls
<b>Pitch</b>: Globally pitch your sample up or down by the semitone. You can also play your sample at different pitches using a MIDI keyboard controller.
<br>

<b>Pan</b>: Pan your sample left or right.
<br>

<b>Level</b>: Control how loud your samples play back.
<br>

<b>Humanize</b>: Humanize your note repeats by slightly randomizing the level of each sample playback.
<br>

<b>Swing</b>: Swing your note repeats. Does not apply when you're recording triplets.
<br>

<b>ADSR</b>: Control the attack, delay, release, and sustain of your note repeats. These settings apply on each sample, so if you turn the attack time up (slower attack), you'll hear its effect every time your sample repeats.
<br>

### Other Controls
<b>Keyswitch Octave</b>: Adjust where the designated keyswitches are to better suit the size of your keyboard.