Argento Luciano (luciano.argento@unical.it)
July 2017


---------------------------------------------------------------------------------
  PCkAD Introduction
---------------------------------------------------------------------------------

Authors:
	Fabrizio Angiulli,
	Luciano Argento,
	Angelo Furfaro


PLEASE CITE US if you use PCkAD for research purposes. The paper to cite is called "Exploiting content spatial distribution to improve detection of intrusions".


Brief description:
PCkAD is a novel semi-supervised anomaly-based IDS (Intrusion Detection System) technique, 
which is able to detect application level content-based attacks, such as web attacks. 
In particular, PCkAD learns legitimate traffic by splitting network packet payloads
in portions of equal length, called chunks, and determining the within packet distribution of n-grams. 
This strategy makes PCkAD resistant to evasion techniques as blending.


NOTE 1: This is still a developmental version of PCkAD. Efforts
have been made to ensure that all bugs are hammered out but there
still could be unexpected segfaults. If you do find any bugs during
runtime, please let me know (luciano.argento@unical.it).

NOTE 2: The repository contains almost all the core modules of PCkAD, 
therefore the technique cannot be employed yet. The remaning modules will be added in the next few days.

NOTE 3: The README is undergoing refinement.


---------------------------------------------------------------------------------
  PCkAD Dependencies
---------------------------------------------------------------------------------
libpcap: for reading pcap network packets
libxml: for parsing XML configurations files

---------------------------------------------------------------------------------
  PCkAD Installation
---------------------------------------------------------------------------------

TODO 

---------------------------------------------------------------------------------
  PCkAD Configuration
---------------------------------------------------------------------------------
The core module of PCkAD can be configured by means of the following configuration files:
GLOBAL.xml:
	n: the n-gram length (bytes)
	lck: the chunk length (bytes)
	strategy: classification strategy. 0 = GS, 1 = LS, 2 = 2LS
	format: the packet format. 0 = PCAP, 1 = TXT

	Targets: list of protocols to analyse
		protocol ID (to be removed): 0 = HTTP, ? = FTP
		port dir: packet direction. 0 = outgoing, 1 = incoming
		port: the tcp port. Supported ports: 80 (HTTP), 21 (FTP)
		threshold: used for classifying a packet payload. It takes values from the range [0, 1]
TRAINING.xml:
	storemodels: it is used to configure PCkAD so that it stores the models created during the training phase. 1 = yes, 0 = no
DETECTION.xml:
	supmodels: it is used to configure PCkAD so that it uses models that partially match the profile of a packet, when needed. Values: 1 = enable, 0 = disable
	alerts: 1 = enable, 0 = disable
	mechanism: this parameter should be removed in the future. Set it to 0.

The libraries that implement the inspection of a packet payload, based on a 
chosen protocol, can be configured with the following files:
FTP.xml: for another is nothing to set
HTTP.xml:
	onlyparams: 1 = the URL is not part of the relevant payload, 0 = the URL is part of the relevant payload

	targets: list of fields to analyse
		type: 0 = GET, 3 = POST, 9: = fragment

The library that implements the computation of the anomaly score of a packet 
payload can be configured with the following file:
ASCMPT.xml:
	ngthreshold: recommended value = 2.0
	distance: 0 is the only valid value, for now
	strcmp: this parameter should be removed in the future. Set it to 0.



REMARKS:
PCkAD may need to undergo a tuning process. However, the experimental results 
reported in "Exploiting content spatial distribution to improve detection of 
intrusions" provide valuable suggestions for the selection of parameter values. 
For instance, it has been observed that PCkAD tend to perform very well when n 
is either set to 3 or 4 and lck takes values from the range [15, 70].

The strategy GS was developed only for experimental purposes, therefore LS or 2LS 
should be used when deploying PCkAD. The performances of the latter two strategies 
are comparable; 2LS might be slightly faster.

---------------------------------------------------------------------------------
  PCkAD Training
---------------------------------------------------------------------------------

TODO 

---------------------------------------------------------------------------------
  PCkAD Classification
---------------------------------------------------------------------------------

TODO 


Please direct all questions and comments to: luciano.argento@unical.it
