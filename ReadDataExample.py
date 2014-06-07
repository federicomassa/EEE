# This program reads the data stored 
# to execute: python ReadData.py
#!/usr/bin/python
import os
import ROOT 
# 
# Definitions
# Chamber vertical position
Zch1 = 145
Zch2 = 85
Zch3 = 23
#
NGood = 0
myDump = 0
nEve = 0
# The file for the old chambers
InputPath = "/media/068E38A68E388FE3/Federico/Universitas/Laboratorio/4.EEE/"
#InputFile = "EEE_NewChambers_20131120_155400.txt"
InputFile = "EEE_PISA01TestRun4Telescopes_20140507_014455.txt"
InputFP = InputPath+InputFile
OutputFP = InputFP+"ch.root"
print ("Input file: ", InputFP)
# ... and the file to save the histograms
hfile = ROOT.TFile(OutputFP, 'RECREATE', 'Demo ROOT file with histograms' )
# Histogramming
hx1=ROOT.TH1F("Chamber1 x - z=145","theTitle;x / [cm]; Entries",100,-100,100)
hy1=ROOT.TH1F("Chamber1 y - z=145","theTitle;y / [cm]; Entries",100,-300,300)
hxy1 = ROOT.TH2F('Chamber1 x.y', 'Chamber z=145 - y vs x', 100, -100, 100, 100, -300,300 )

hx2=ROOT.TH1F("Chamber2 x - z=85","theTitle;x / [cm]; Entries",100,-100,100)
hy2=ROOT.TH1F("Chamber2 y - z=85","theTitle;y / [cm]; Entries",100,-300,300)
hxy2 = ROOT.TH2F('Chamber2 x.y', 'Chamber z=85 - y vs x', 100, -100, 100, 100, -300,300 )

hx3=ROOT.TH1F("Chamber3 x - z=23","theTitle;x / [cm]; Entries",100,-100,100)
hy3=ROOT.TH1F("Chamber3 y - z=23","theTitle;y / [cm]; Entries",100,-300,300)
hxy3 = ROOT.TH2F('Chamber3 x.y', 'Chamber z=23 - y vs x', 100, -100, 100, 100, -300,300 )

c1 = ROOT.TCanvas()
func = ROOT.TF1("PROOVA","x+1",-100,100)

prova = ROOT.TH1F("Prova","Title;x;y",10,-100,100)
for i in range(0,10):
    prova.SetBinContent(i,i+1)


#
# Start reading the file
source = open(InputFP,'r') 
# Start to read the data file
for line in source:
    if myDump==1: print (line[11:14])
    if line[0:3] == 'Run':
        if myDump ==1: print (line) 
    else: 
        if line[11:14]=='GPS':
            print ('this is the GPS line')
        if line[11:14]=='EVE':
# Found one event
            nEve += 1
            if myDump==1: 
                print ('this is a new event: ',nEve)
                print ('this is an event line')
                print (line[16:])
# Define an empty vector
            theNumbers = []
# Read the numbers stored in the lines after the EVENT tag
            for val in line[16:].split():
                theNumbers.append(float(val))
            if myDump==1: print (theNumbers,len(theNumbers))
# Now filling the histogram
            for i in range(6,len(theNumbers),3):
                if myDump==1: print ('theNumbers[i],i',theNumbers[i],i)
                if theNumbers[i+2]==Zch1: 
                    hx1.Fill(theNumbers[i])
                    hy1.Fill(theNumbers[i+1])
                    hxy1.Fill(theNumbers[i],theNumbers[i+1])
                if theNumbers[i+2]==Zch2: 
                    hx2.Fill(theNumbers[i])
                    hy2.Fill(theNumbers[i+1])
                    hxy2.Fill(theNumbers[i],theNumbers[i+1])
                if theNumbers[i+2]==Zch3: 
                    hx3.Fill(theNumbers[i])
                    hy3.Fill(theNumbers[i+1])
                    hxy3.Fill(theNumbers[i],theNumbers[i+1])
                    
                
#
print ('Read out ',nEve,' events')
print ("Output file: ", OutputFP)
hfile.Write()
#Always close the file 
source.close()

