# this script is called once for each reco bin (obsBin)
# in each reco bin there are (nBins) signals (one for each gen bin)

import optparse
import ROOT
import os,sys, string, math, pickle
from ROOT import *

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

parser = optparse.OptionParser()
parser.add_option("--cutChain",dest="cutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
parser.add_option("--doSmoothing",action="store_true", dest="doSmoothing", default=False,help="")
parser.add_option("--dyMC",action="store_true", dest="dyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)

mass=[600,800,1000,1200,1400,1600,1800,2000]

def createXSworkspace(MX,channel,cat, parser):

    (options,args) = parser.parse_args()

    cut=options.cutChain
    isDYMC=options.dyMC
    isGJets= (not isDYMC)
    doSmoothing=options.doSmoothing

    zjetsMethod = 'mczjet'
    if( not isDYMC) :
      zjetsMethod = 'gjet'

    outdir = zjetsMethod
    outdir = outdir +'_'
    outdir = outdir + cut
    outdir = outdir + '_'
    if(doSmoothing) :
      outdir = outdir + 'Smooth' 
    else :
      outdir = outdir + 'unSmooth'

    if not os.path.exists(outdir):
       print 'mkdir -p ',outdir
       os.system('mkdir -p '+outdir)

    prefix = "GJets_RhoWt_GMCEtaWt_tight"+cut+"_puWeightmoriondMC_sys_muoneg_"+zjetsMethod+"_metfilter_unblind"
    if cat == 'cr1' :
       prefix = "GJets_RhoWt_GMCEtaWt_antitight"+cut+"_CR1_puWeightmoriondMC_sys_muoneg_"+zjetsMethod+"_metfilter_unblind"
    if cat == 'cr2' :
       prefix = "GJets_RhoWt_GMCEtaWt_antitight"+cut+"_CR2_puWeightmoriondMC_sys_muoneg_"+zjetsMethod+"_metfilter_unblind"
    if cat == 'cr3' :
       prefix = "GJets_RhoWt_GMCEtaWt_antitight"+cut+"_CR3_puWeightmoriondMC_sys_muoneg_"+zjetsMethod+"_metfilter_unblind"

    fs=""
    if(channel=="mm") :   
      fs = "mu"
    elif(channel=="ee") :
      fs = "el"

    inputfileName = prefix + "_" + fs + "_log_1pb"
    print 'fs ',fs,' cut ',cut,' at MX =',MX,'GeV' 
    inputfile = TFile(inputfileName+".root","READ")

    #####
    Shape = {}
    Shape_ups = {}
    Shape_dns = {}

    theRates = {}

    sigName = 'BulkGravToZZToZlepZinv_narrow_'+str(MX)+'_BulkGravToZZToZlepZinv_narrow_'+str(MX)
    zjetsName = 'ZJets_DYJetsToLL_M50_BIG_Rc36p46DtReCalib'
    if(isGJets):
       zjetsName="ZJets_SinglePhoton_Run2016B2H_ReReco_36p46_Rc36p46ReCalib"

    processes = ['NonReso_muonegtree_light_skim','VVZReso_WZTo2L2Q',zjetsName, sigName]
    #process xzz zjets vvreso nonreso
    systs = ['fidxsec','id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']
    systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']

    processNameinDC = {'NonReso_muonegtree_light_skim':'nonreso','VVZReso_WZTo2L2Q':'vvreso',zjetsName:'zjets', sigName:'xzz'}

    print 'import histograms from ',inputfileName   

    n1_nonreso = 10
    n1 = 17
    n2 = 21

    if(cat=='cr1') :
     n1 = 9
     n2 =10
     n1_nonreso = 6 

    if(cat=='cr2') :
     n1 = 3
     n2 = 4
     n1_nonreso = 6

    for process in processes :

       print process
       Shape[process] = (inputfile.Get(inputfileName+"_mT_"+process)).Clone()
       Shape[process].SetName(processNameinDC[process])
       Shape[process].SetTitle(processNameinDC[process])

       if(doSmoothing) :

         if(process=='NonReso_muonegtree_light_skim'):
           rebinNonReso(Shape[process],n1_nonreso)
         if(process=='VVZReso_WZTo2L2Q' or process==zjetsName):
           rebin(Shape[process],n1,n2)

       for syst in systs :

         Shape_dns[syst+"_"+process] = (inputfile.Get(inputfileName+"_mT_"+syst+"Dn_"+process)).Clone()
         Shape_ups[syst+"_"+process] = (inputfile.Get(inputfileName+"_mT_"+syst+"Up_"+process)).Clone()

         if(doSmoothing) :         

           if(process=='NonReso_muonegtree_light_skim'):
             rebinNonReso(Shape_dns[syst+"_"+process],n1_nonreso)
             rebinNonReso(Shape_ups[syst+"_"+process],n1_nonreso)
           if(process=='VVZReso_WZTo2L2Q' or process==zjetsName):
             rebin(Shape_dns[syst+"_"+process],n1,n2)
             rebin(Shape_ups[syst+"_"+process],n1,n2)

         if(syst=="trg" or syst=="id" or syst=="zpt" or syst=="fidxsec") :
           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+syst+fs+"Down")
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+syst+fs+"Up")
         elif(syst in systsMT and processNameinDC[process]!='xzz' and processNameinDC[process]!='vvreso'):        
           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+syst+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+syst+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+processNameinDC[process]+syst+"Down")
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+processNameinDC[process]+syst+"Up")
         else :        
           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+syst+"Down")
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process]+"_"+syst+"Up")

       #print 'histograms : fill empty bins'
       for i in range(0,Shape[process].GetXaxis().GetNbins()) :

          if(Shape[process].GetBinContent(i+1)<1e-30) :
            Shape[process].SetBinContent(i+1,1e-30)
 
          for syst in systs :

            if(Shape_dns[syst+"_"+process].GetBinContent(i+1)<1e-30) : 
               Shape_dns[syst+"_"+process].SetBinContent(i+1,1e-30)
            if(Shape_ups[syst+"_"+process].GetBinContent(i+1)<1e-30) : 
               Shape_ups[syst+"_"+process].SetBinContent(i+1,1e-30)

       theRates[process] = Shape[process].Integral()

       # MC statistical uncertainty
       if(process=='VVZReso_WZTo2L2Q' or process=='NonReso_muonegtree_light_skim'):

         Shape_dns["statUnc_"+process] = Shape[process].Clone()
         Shape_ups["statUnc_"+process] = Shape[process].Clone()

         Shape_dns["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Down")
         Shape_ups["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Up")
         Shape_dns["statUnc_"+process].SetTitle(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Down")
         Shape_ups["statUnc_"+process].SetTitle(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Up")

         print 'MC statistical uncertainty for ',process
         for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :

           dn = max(1e-30,Shape[process].GetBinContent(i)-0.5*Shape[process].GetBinError(i))
           up = Shape[process].GetBinContent(i)+0.5*Shape[process].GetBinError(i)
           Shape_dns["statUnc_"+process].SetBinContent(i,dn)
           Shape_ups["statUnc_"+process].SetBinContent(i,up)

    print 'signal scale from fb to pb'
    #########
    Shape[sigName].Scale(0.001)
    for syst in systs :
      Shape_dns[syst+"_"+sigName].Scale(0.001)
      Shape_ups[syst+"_"+sigName].Scale(0.001)

    theRates[sigName] = Shape[sigName].Integral()
    
    ## obs data
    print 'import data'
    data_obs = (inputfile.Get(inputfileName+"_mT_data_SingleEMU_Run2016B2H_ReReco_36p46_DtReCalib")).Clone()
    data_obs.SetName("data_obs")
    data_obs.SetTitle("data_obs")
    Nobs = data_obs.Integral()

    ## obs nonreso CR data
    print 'import CR data'
    dataCR_obs = (inputfile.Get(inputfileName+"_mT_NonResoCR_muonegtree_light_skim")).Clone()
    dataCR_obs.SetName("dataCR")
    dataCR_obs.SetTitle("dataCR")
    NnonresoCR = int(dataCR_obs.Integral()) 
    print 'NnonresoCR ',NnonresoCR

    #### out #### 
    print 'write histograms to rootfile (workspace)'
    outfile = TFile(outdir+"/mX"+str(MX)+"ZZ2l2nu_"+channel+"_"+cut+"_"+cat+".root","RECREATE")
    outfile.cd()

    for process in processes :

      Shape[process].Write()
      for syst in systs :

        Shape_dns[syst+"_"+process].Write()
        Shape_ups[syst+"_"+process].Write()

      if(process=='VVZReso_WZTo2L2Q' or process=='NonReso_muonegtree_light_skim'):

        Shape_dns["statUnc_"+process].Write()
        Shape_ups["statUnc_"+process].Write()

    data_obs.Write()
    outfile.Close()

    ## Write Datacards 
    fo = open( outdir+"/mX"+str(MX)+"ZZ2l2nu_"+channel+"_"+cut+"_"+cat+".txt", "wb")
    WriteDatacard(fo,MX,channel,cut,theRates,Nobs,NnonresoCR,cat,n1,n2, isGJets)
    fo.close()


def WriteDatacard(theFile,MX,channel,cut,theRates,obsEvents,NnonresoCR,cat,n1,n2, isGJets):

        numberSig = 1
        numberBg  = len(theRates)-1

        fs="el"
        if(channel=="mm") :
          fs = "mu"
        elif(channel=="ee") :
          fs = "el"

        theFile.write("imax 1\n")
        theFile.write("jmax {0}\n".format(numberSig+numberBg-1))
        theFile.write("kmax *\n")

        theFile.write("------------\n")
        theFile.write("shapes * * mX{0}ZZ2l2nu_{1}_{2}_{3}.root $PROCESS $PROCESS_$SYSTEMATIC \n".format(str(MX),channel,cut,cat))
        theFile.write("------------\n")
        theFile.write("bin {0}_{1} \n".format(channel,cat))
        theFile.write("observation {0} \n".format(obsEvents))
        theFile.write("------------\n")
        theFile.write("bin ")

        sigName = 'BulkGravToZZToZlepZinv_narrow_'+str(MX)+'_BulkGravToZZToZlepZinv_narrow_'+str(MX)
        zjetsName = 'ZJets_DYJetsToLL_M50_BIG_Rc36p46DtReCalib'
        if(isGJets):
          zjetsName="ZJets_SinglePhoton_Run2016B2H_ReReco_36p46_Rc36p46ReCalib"

        processList = [sigName,zjetsName,'VVZReso_WZTo2L2Q','NonReso_muonegtree_light_skim']
        processName1D=['xzz','zjets','vvreso','nonreso']

        for process in processList:
            theFile.write("{0}_{1} ".format(channel,cat))
        theFile.write("\n")

        theFile.write("process ")

        i=0
        for process in processList:
            theFile.write("{0} ".format(processName1D[i]))
            i+=1

        theFile.write("\n")

        processLine = "process "

        for x in range(-numberSig+1,1):
            processLine += "{0} ".format(x)

        for y in range(1,numberBg+1):
            processLine += "{0} ".format(y)

        theFile.write(processLine)
        theFile.write("\n")

        theFile.write("rate ")
        for process in processList:
            theFile.write("{0:.4f} ".format(theRates[process]))
        theFile.write("\n")
        theFile.write("------------\n")

        ### sys unc ###

        '''

- PDF:

Signal:
mass:      600    800  1000  1200  1400  1600  1800  2000  2500  3000  3500  4000
uncert:  0.090 0.106 0.124 0.144 0.167 0.193 0.220 0.251 0.343 0.427 0.522 0.601

BKG:         ZJets   VVZReso  NonReso
Uncertainty  0.033    0.015   0.013

- QCD:

Signal:
mass:      600   800  1000  1200  1400  1600  1800  2000  2500  3000  3500  4000
Uncert.  0.075 0.091 0.104 0.114 0.123 0.130 0.137 0.143 0.156 0.168 0.179 0.189

        '''

        sigPDF = {'600':1.090, '800':1.106, '1000':1.124, '1200':1.144, '1400':1.167, '1600':1.193, '1800':1.220, '2000':1.251, '2500':1.343, '3000':1.427, '3500':1.522, '4000':1.601}

        sigQCD = {'600':1.075, '800':1.091, '1000':1.104, '1200':1.114, '1400':1.123, '1600':1.130, '1800':1.137, '2000':1.143, '2500':1.156, '3000':1.168, '3500':1.179, '4000':1.189}        

        theFile.write("# norm uncertainty \n")
        theFile.write("lumi_13TeV lnN 1.06 - 1.06 - \n")
        theFile.write("pdf_qqbar lnN - 1.017 1.015 - \n")
        theFile.write("xzz2l2nu_accept lnN 1.01 - - - \n")
        theFile.write("trg{0} shapeN2 1.0 - 1.0 - \n".format(fs))
        theFile.write("id{0} shapeN2  1.0 - 1.0 - \n".format(fs))
        theFile.write("# photon+jets zpt reweight uncertainty \n")         
        theFile.write("fidxsec{0} shapeN2 - 0.5 - - \n".format(fs))

        ## non-res and z+jets are data-driven now, no direct trig/id uncertainty
        theFile.write("#non-res syst unc \n")
        unc_tmp = 1.05
        if(channel=="ee") :
          unc_tmp = 1.043
        elif(channel=="mm") :
          unc_tmp = 1.01
        theFile.write("idnonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))
        unc_tmp = 1.02
        theFile.write("scalenonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))
        theFile.write("#non-res stat unc \n") 
        theFile.write("nonresoStatUnc{0}{1} shapeN2 - - - 1.0 \n".format(cat,fs))

        theFile.write("#MET-induced MT unc \n")
        systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster']
        for syst in systsMT :
           theFile.write("{0} shapeN2 1.0 - 1.0 - \n".format(syst))

        theFile.write("zjetsRecoil shapeN2 - 1.0 - - \n".format(syst))

        theFile.write("#MT MC stat unc \n")
        theFile.write("vvresoStatUnc{0}{1} shapeN2 - - 1.0 - \n".format(cat,fs))
        

def rebinNonReso(hist,n1) :

    Nbins = hist.GetXaxis().GetNbins()
    bincontent = 0.0
    binerror = 0.0

    for i in range(n1,Nbins+1):
     bincontent = bincontent + hist.GetBinContent(i)
     binerror = binerror + hist.GetBinError(i)*hist.GetBinError(i)

    bincontent = bincontent/(Nbins+1-n1)
    binerror = pow(binerror,0.5)/(Nbins+1-n1)     

    for i in range(n1,Nbins+1):
     hist.SetBinContent(i,bincontent)
     hist.SetBinError(i,binerror)

def rebin(hist,n1,n2) :

    Nbins = hist.GetXaxis().GetNbins()
    bincontent = 0.0
    binerror = 0.0
    for i in range(n1,n2):
     bincontent = bincontent + hist.GetBinContent(i)
     binerror = binerror + hist.GetBinError(i)*hist.GetBinError(i)

    bincontent = bincontent/(n2-n1)
    binerror = pow(binerror,0.5)/(n2-n1)

    for i in range(n1,n2):
     hist.SetBinContent(i,bincontent)
     hist.SetBinError(i,binerror)

    bincontent = 0.0
    binerror = 0.0
    for i in range(n2,Nbins+1):
     bincontent = bincontent + hist.GetBinContent(i)
     binerror = binerror + hist.GetBinError(i)*hist.GetBinError(i)

    bincontent = bincontent/(Nbins+1-n2)
    binerror = pow(binerror,0.5)/(Nbins+1-n2)

    for i in range(n2,Nbins+1):
     hist.SetBinContent(i,bincontent)
     hist.SetBinError(i,binerror)


for m in mass :

  createXSworkspace(m, "mm",'sr', parser)
  createXSworkspace(m, "ee",'sr', parser)

#  createXSworkspace(m, "mm",'cr1', parser)
#  createXSworkspace(m, "ee",'cr1', parser)

#  createXSworkspace(m, "mm",'cr2', parser)

