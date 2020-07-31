# Quasar Spectroscopy Sound

This repository contains the software Quasar Spectroscopy Sound version 1.0.0 for performing Quasar absoprtion line sonificaiton. 
![](/READMEImages/QSS_UI.PNG)


The software and implemented sonifciation approaches are described in the paper QUASAR SPECTROSCOPY SOUND: ANALYZING INTERGALACTIC AND CIRCUMGALACTIC MEDIA WITH SOUND (included in this repository as QuasarSpectroscopySound.png). Specifically, reference sections 2 and 3 of the paper to learn more details about the sonification and software functionatliy.


Source code and a compiled binary (Quasar Sepctroscopy Sound) has been included in the repository for demonstration. The software is designed to run on MacOS 10.11 and above.


To test the software, follow the steps below:

Step 1: in the "Data Configuration" secdtion, load a dataset for analysie and select an ion under "Selected Element". Note that the software is preloaded with the spectroscopy dataset "TON580_norm.asii" and by default is set to analyze the element H1 (hydrogen).

Step2: In the "Audio Controls" section, toggle the audio on, and adjust the slider directly to the right to increase the sound volume. By default, the "Presence" audio is enabled for all spectral transitions of the H1 element.

Step 3: In the "Data Naigation" section, explore the dataset by adjusting the horizontal slider directly to the right of the "Anchor Red Shift" text entry box. To closely investigate a spectral location, you may adjust the velociy increment slider ("Vel Incr") and then click and drag the jog wheel left or right to finely scroll about the region. The visual plot at the bottom will assist with navigation, as it helps reveal absoprtion areas in the spectrum.

Further Use:

For any spectral transition, as displayed in the audio controls, you may enable the "Centroid" or "ACD" and listen for these features at a given redshift lcoation. You may also click on the "Width" button to hear an audio display of the spectral width.


