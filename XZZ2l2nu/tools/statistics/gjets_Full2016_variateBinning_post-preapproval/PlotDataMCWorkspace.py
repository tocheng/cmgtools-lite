#!/usr/bin/env python

import ROOT, string
import math
from math import *

from ROOT import *
from array import array

import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re

from tdrStyle import *
setTDRStyle()


grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

def parseOptions():

    usage = ('usage: %prog [options]\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option("-s", "--signal", dest="signal",default='xzz1000',help="signal")
    parser.add_option("-i", "--input", dest="inputfile",default='input.root',help="input root file")
    parser.add_option("-o", "--output", dest="output",default='output',help="output file")
    parser.add_option("--obs", dest="Observable",default='mT',help="template observable")
    #parser.add_option("--unblind",action="store_true", dest="unblind", default=False,help="unblind")

    parser.add_option("-l",action="callback",callback=callback_rootargs)
    parser.add_option("-q",action="callback",callback=callback_rootargs)
    parser.add_option("-b",action="callback",callback=callback_rootargs)

    return parser 

def plotDataMC(file_in, outputplot, plots, observable, signal, shape_type, channel, cat, draw_pull):
    

    bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 600.0, 700.0, 800.0, 900, 1050, 1150, 1250, 1650, 3000.0]
    if(observable=="MET"):
       bins = [0.0, 50, 100.0, 150, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 600, 700, 1000, 1500]

    nbins = len(bins)-1


    # 
    plots.Clear()

    # get shapes from the workspace
    ## obs data in ws
    gr_data_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/data').Clone("gr_data_ws")

    ## total_background
    h_total_background_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/total_background').Clone("h_total_background_ws")

    ## signal
    h_signal_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/'+signal).Clone("h_signal_ws")
    if shape_type=="shapes_fit_b": h_signal_ws = file_in.Get('shapes_prefit/'+channel+'_'+cat+'/'+signal).Clone("h_signal_ws")
 
    ## backgrounds
    h_nonreso_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/nonreso').Clone("h_nonreso_ws")
    h_vvreso_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/vvreso').Clone("h_vvreso_ws") 
    h_zjets_ws = file_in.Get(shape_type+'/'+channel+'_'+cat+'/zjets').Clone("h_zjets_ws") 
 
    # define shapes for drawing
    # hist for data
    h_data = TH1D("h_data", "data", nbins, array('d', bins))
    # total bkg
    h_total_background = TH1D("total_background", "total_background", nbins, array('d', bins))
    # signal and bkgs
    h_signal = TH1D("h_signal", "signal", nbins, array('d', bins))
    h_nonreso = TH1D("h_nonreso", "nonreso", nbins, array('d', bins))
    h_vvreso = TH1D("h_vvreso", "vvreso", nbins, array('d', bins))
    h_zjets = TH1D("h_zjets", "zjets", nbins, array('d', bins))
    h_signal.Sumw2()
    h_nonreso.Sumw2()
    h_vvreso.Sumw2()
    h_zjets.Sumw2()
    
    # data
    data_d_array_y = array('d', [gr_data_ws.GetY()[ib]/(bins[ib+1]-bins[ib])*50.0 for ib in range(nbins)])
    data_d_array_eyh = array('d', [gr_data_ws.GetEYhigh()[ib]/(bins[ib+1]-bins[ib])*50.0 for ib in range(nbins)])
    data_d_array_eyl = array('d', [gr_data_ws.GetEYlow()[ib]/(bins[ib+1]-bins[ib])*50.0 for ib in range(nbins)])
    data_d_array_x = array('d', [(bins[ib]+bins[ib+1])/2 for ib in range(nbins)])
    data_d_array_exh = array('d', [bins[ib+1]-(bins[ib]+bins[ib+1])/2 for ib in range(nbins)])
    data_d_array_exl = array('d', [-bins[ib] +(bins[ib]+bins[ib+1])/2 for ib in range(nbins)])
    gr_data = TGraphAsymmErrors(nbins,data_d_array_x,data_d_array_y,data_d_array_exl,data_d_array_exh,data_d_array_eyl,data_d_array_eyh)
    gr_data.SetName("gr_data")
    for ip in range(nbins): 
        if gr_data.GetY()[ip]<=0.0: 
            gr_data.SetPointEYhigh(ip,0.0)   
            gr_data.SetPointEYlow(ip,0.0)   

    # ratio bins
    r_bins = [0,0.001]+bins[1:-2] + [bins[-1]-0.001,bins[-1]] # extend lower and higher bins
    r_nbins = len(r_bins)-1
   
    print r_bins

    # uncertainty bands
    h_band_sys = TH1D('h_band_sys', '',r_nbins,array('d',r_bins))
    h_band_all = TH1D('h_band_all', '',r_nbins,array('d',r_bins))

    for i in range(1, nbins+1) :
        # h_data
        data_cnt=data_d_array_y[i-1]
        data_err=(data_d_array_eyh[i-1]+data_d_array_eyl[i-1])/2.0
        if data_cnt<=0.0:  data_err = 0.0;
        h_data.SetBinContent(i, data_cnt)
        h_data.SetBinError(i, data_err)

        # total background
        h_total_background.SetBinContent(i, h_total_background_ws.GetBinContent(i)/(bins[i]-bins[i-1])*50.0)
        h_total_background.SetBinError(i, h_total_background_ws.GetBinError(i)/(bins[i]-bins[i-1])*50.0)
        # signal and bkg
        h_signal.SetBinContent(i, h_signal_ws.GetBinContent(i)/(bins[i]-bins[i-1])*50.0)
        h_signal.SetBinError(i, h_signal_ws.GetBinError(i)/(bins[i]-bins[i-1])*50.0)
        h_nonreso.SetBinContent(i, h_nonreso_ws.GetBinContent(i)/(bins[i]-bins[i-1])*50.0)
        h_nonreso.SetBinError(i, h_nonreso_ws.GetBinError(i)/(bins[i]-bins[i-1])*50.0)
        h_vvreso.SetBinContent(i, h_vvreso_ws.GetBinContent(i)/(bins[i]-bins[i-1])*50.0)
        h_vvreso.SetBinError(i, h_vvreso_ws.GetBinError(i)/(bins[i]-bins[i-1])*50.0)
        h_zjets.SetBinContent(i, h_zjets_ws.GetBinContent(i)/(bins[i]-bins[i-1])*50.0)
        h_zjets.SetBinError(i, h_zjets_ws.GetBinError(i)/(bins[i]-bins[i-1])*50.0)
        # error band ratio plot
        h_band_sys.SetBinContent(i+1,1)
        h_band_sys.SetBinError(i+1,h_total_background_ws.GetBinError(i)/h_total_background_ws.GetBinContent(i))
        h_band_all.SetBinContent(i+1,1)
        h_band_all.SetBinError(i+1,sqrt(pow(h_total_background_ws.GetBinError(i)/h_total_background_ws.GetBinContent(i),2)+pow((data_d_array_eyh[i-1]+data_d_array_eyl[i-1])/2.0/max(data_d_array_y[i-1],0.00001),2))) 
        # fill lowest and highest bin
        if i==1 : 
            h_band_sys.SetBinContent(1,1)
            h_band_sys.SetBinError(1,h_band_sys.GetBinError(i+1))
            h_band_all.SetBinContent(1,1)
            h_band_all.SetBinError(1,h_band_all.GetBinError(i+1))
        if i==nbins :
            h_band_sys.SetBinContent(r_nbins,1)
            h_band_sys.SetBinError(r_nbins,h_band_sys.GetBinError(i+1))
            h_band_all.SetBinContent(r_nbins,1)
            h_band_all.SetBinError(r_nbins,h_band_all.GetBinError(i+1))

    h_band_sys.SetMarkerColor(0)
    h_band_sys.SetMarkerColor(kGreen+3)
    h_band_sys.SetLineColor(kGreen+3)
    h_band_sys.SetFillColor(kGreen+3)
    h_band_sys.SetFillStyle(3001)

    h_band_all.SetMarkerColor(0)
    h_band_all.SetMarkerColor(kGreen+1)
    h_band_all.SetLineColor(kGreen+1)
    h_band_all.SetFillColor(kGreen+1)
    h_band_all.SetFillStyle(3002)

    h_signal.SetMarkerColor(kRed+2)
    h_signal.SetLineColor(kRed+2)
    h_signal.SetFillColor(kRed+2)
    if shape_type=="shapes_fit_s": h_signal.SetFillStyle(3001)
    elif shape_type=="shapes_prefit": h_signal.SetFillStyle(0)
    elif shape_type=="shapes_fit_b": h_signal.SetFillStyle(0)
    h_signal.SetLineStyle(1)
    h_signal.SetLineWidth(3)


    h_nonreso.SetMarkerColor(kOrange)
    h_nonreso.SetLineWidth(3)
    h_nonreso.SetFillColor(kOrange)

    h_vvreso.SetMarkerColor(kMagenta)
    h_vvreso.SetLineWidth(3)
    h_vvreso.SetFillColor(kMagenta)

    h_zjets.SetMarkerColor(kGreen+2)
    h_zjets.SetLineWidth(3)
    h_zjets.SetFillColor(kGreen+2)


    #plotting data/MC

    dummy = TH1D("dummy","dummy", 1, 0,bins[nbins])

    dummy.SetBinContent(1,0.0)
    dummy.GetXaxis().SetTitle(observable+' (GeV)')
    if observable=="MET": dummy.GetYaxis().SetTitle('Events / 50 GeV')
    else: dummy.GetYaxis().SetTitle('Events / 50 GeV') 
    dummy.SetLineColor(0)
    dummy.SetLineWidth(0)
    dummy.SetFillColor(0)
    dummy.SetMinimum(0.001)
    dummy.SetMaximum(1000000.0)
    dummy.GetXaxis().SetMoreLogLabels(kTRUE)
    dummy.GetXaxis().SetNoExponent(kTRUE)
    dummy.GetXaxis().SetTitleSize(0)
    dummy.GetXaxis().SetLabelSize(0)
    dummy.Draw()


    stack = THStack("stack","stack")   
    stack.Add(h_nonreso)
    stack.Add(h_vvreso)
    stack.Add(h_zjets)
    stack.Draw("histsame")
    if shape_type=="shapes_fit_s": stack.Add(h_signal) 

    stack.GetXaxis().SetMoreLogLabels(kTRUE)
    stack.GetXaxis().SetNoExponent(kTRUE)
    stack.GetXaxis().SetTitleSize(0)
    stack.GetXaxis().SetLabelSize(0)

    gr_data.SetMarkerStyle(20)
    gr_data.SetMarkerSize(1)
    #data_obs_orig.SetBinErrorOption(TH1.kPoisson)
    gr_data.Draw("p same")

    #hmask_data.Draw("HIST,SAME")

    if shape_type=="shapes_prefit": h_signal.Draw("HIST SAME")
    elif shape_type=="shapes_fit_b": h_signal.Draw("HIST SAME")
    
    legend = TLegend(.55,.6,.90,.90)
    legend.AddEntry(h_zjets, 'Z+jets', "f")
    legend.AddEntry(h_vvreso, 'Reson. backgrounds', "f")
    legend.AddEntry(h_nonreso, 'Non-Reson. backgrounds', "f")
    if shape_type=="shapes_prefit": legend.AddEntry(h_signal, '1 pb BulkG M = 1 TeV', "f")
    elif shape_type=="shapes_fit_b": legend.AddEntry(h_signal, '1 pb BulkG M = 1 TeV', "f")
    elif shape_type=="shapes_fit_s": legend.AddEntry(h_signal, 'BulkG M = 1 TeV', "f")
    legend.AddEntry(gr_data, 'Data', "pl")
    legend.SetShadowColor(0);
    legend.SetFillColor(0);
    legend.SetLineColor(0);
    legend.Draw("same")

    latex2 = TLatex()
    latex2.SetNDC()
    latex2.SetTextSize(0.5*plots.GetTopMargin())
    latex2.SetTextFont(42)
    latex2.SetTextAlign(31) # align right
    latex2.DrawLatex(0.95, 0.94,"#sqrt{s} = 13 TeV 2016 L = 35.9 fb^{-1}")
    latex2.SetTextSize(0.4*plots.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11) # align right
    latex2.DrawLatex(0.2, 0.94, "CMS")
    latex2.SetTextSize(0.4*plots.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11)
#    latex2.DrawLatex(0.27, 0.94, "Preliminary")

    if shape_type=="shapes_prefit": latex2.DrawLatex(0.27, 0.85, "Pre-fit")
    elif shape_type=="shapes_fit_b": latex2.DrawLatex(0.27, 0.85, "Post-fit B-only")
    elif shape_type=="shapes_fit_s": latex2.DrawLatex(0.27, 0.85, "Post-fit S+B")

    latex2.SetTextFont(42)
    if channel=="mm": ch_to_plot="#mu#mu channel"
    elif channel=="ee": ch_to_plot="ee channel"
    else: ch_to_plot=" "
    latex2.DrawLatex(0.27, 0.81, ch_to_plot)    

    gPad.RedrawAxis()

    pad = TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
    pad.SetTopMargin(0.71);
    pad.SetRightMargin(0.05);
    pad.SetFillColor(0);
    #pad.SetGridy(1);
    pad.SetFillStyle(0);
    if draw_pull: pad.SetGrid(0,1)
    pad.Draw();
    pad.cd(0);

    dummyR = TH1D("dummyR","dummyR", 1, 0,bins[nbins])
    dummyR.SetBinContent(1,0.0)
    dummyR.GetXaxis().SetTitle(observable+' (GeV)')
    dummyR.GetXaxis().SetMoreLogLabels(kTRUE)
    dummyR.GetXaxis().SetNoExponent(kTRUE)
    dummyR.GetYaxis().SetTitleSize(0.04);
    dummyR.GetYaxis().SetTitleOffset(1.8);
    dummyR.GetYaxis().CenterTitle();
    dummyR.GetYaxis().SetLabelSize(0.02);
    if observable=="mT": dummyR.GetXaxis().SetTitle("M_{T} (GeV)")
    elif observable=="MET": dummyR.GetXaxis().SetTitle("E_{T}^{miss} (GeV)")
    else: dummyR.GetXaxis().SetTitle(observable+"(GeV)")


    # a line at the center of the ratio/pull plot
    h_line = dummyR.Clone('h_line')
    h_line.SetBinError(1,0)
    h_line.SetLineColor(2)
    h_line.SetLineWidth(1)
    h_line.SetFillStyle(0)
    h_line.SetFillColor(0)

    if draw_pull:
        # frame
        dummyR.GetYaxis().SetTitle("#frac{Data-Bkg.}{#sigma_{sys.+stat.}}");
        dummyR.SetMinimum(-5)
        dummyR.SetMaximum(5)
        dummyR.Draw()
        # line should at 0
        h_line.SetBinContent(1,0.0)
        h_line.Draw("hist same")
        # band 2sigma
        h_band_2sigma = dummyR.Clone('h_band_2sigma')
        h_band_2sigma.SetBinContent(1,0)
        h_band_2sigma.SetBinError(1,2.0)
        h_band_2sigma.SetMarkerColor(kRed)
        h_band_2sigma.SetLineColor(kRed+3)
        h_band_2sigma.SetFillColor(kRed+3)
        h_band_2sigma.SetFillStyle(3003)
        h_band_2sigma.Draw("e2same")
        # band 1sigma
        h_band_1sigma = dummyR.Clone('h_band_1sigma')
        h_band_1sigma.SetBinContent(1,0)
        h_band_1sigma.SetBinError(1,1.0)
        h_band_1sigma.SetMarkerColor(kRed+2)
        h_band_1sigma.SetLineColor(kRed+2)
        h_band_1sigma.SetFillColor(kRed+2)
        h_band_1sigma.SetFillStyle(3002)
        h_band_1sigma.Draw("e2same")
        # create pull
        h_pull = h_data.Clone('h_pull')
        h_pull.SetMarkerStyle(20)
        h_pull.SetMarkerColor(1)
        h_pull.SetLineColor(1)
        h_pull.SetLineStyle(1)
        for ib in range(nbins): 
            h_pull.SetBinContent(ib+1, (h_data.GetBinContent(ib+1)-h_total_background.GetBinContent(ib+1))/sqrt(pow(h_data.GetBinError(ib+1),2)+pow(h_total_background.GetBinError(ib+1),2)))
            h_pull.SetBinError(ib+1, 1.0)
        h_pull.Draw("epsame")
        #legend
        legend2 = TLegend(.65,.233,.75,.267)
        legend2.AddEntry(h_band_1sigma, '1-#sigma', "f")
        legend2.SetFillStyle(0);
        legend2.SetFillColor(0);
        legend2.SetLineStyle(0);
        legend2.SetLineColorAlpha(0,0);
        legend2.Draw("same")
        legend3 = TLegend(.72,.233,.82,.267)
        legend3.AddEntry(h_band_2sigma, '2-#sigma', "f")
        legend3.SetFillStyle(0);
        legend3.SetFillColor(0);
        legend3.SetLineStyle(0);
        legend3.SetLineColorAlpha(0,0);
        legend3.Draw("same")
    else:
        # frame 
        dummyR.GetYaxis().SetTitle("Data/Bkg.");
        dummyR.SetMinimum(0.0)
        dummyR.SetMaximum(2.0)
        dummyR.Draw()
        # line should at 1
        h_line.SetBinContent(1,1.0)
        h_line.Draw("hist, same")
        # draw error band only if draw ratio
        #h_band_all.Draw("e3same")
        h_band_sys.Draw("e3same")
        # create and draw ratio
        h_ratio = h_data.Clone('h_ratio')
        h_ratio.Divide(h_total_background)
        # reset empty data bin to -1 with zero error
        for ibin in range(1, h_ratio.GetNbinsX()+1):
            if h_ratio.GetBinContent(ibin)<=0.0: 
                h_ratio.SetBinContent(ibin, -1)
                h_ratio.SetBinError(ibin, 0)
        h_ratio.SetLineColor(1)
        h_ratio.SetMarkerColor(1)
        h_ratio.SetMarkerStyle(20)
        h_ratio.SetLineStyle(1)
        h_ratio.Draw("epsame");
        #legend
        legend2 = TLegend(.60,.233,.8,.267)
        legend2.AddEntry(h_band_sys, 'sys. unc.', "f")
        legend2.SetFillStyle(0);
        legend2.SetFillColor(0);
        legend2.SetLineStyle(0);
        legend2.SetLineColorAlpha(0,0);
        legend2.Draw("same")
        legend3 = TLegend(.74,.233,.94,.267)
        #legend3.AddEntry(h_band_all, 'sys.+stat. unc.', "f")
        legend3.SetFillStyle(0);
        legend3.SetFillColor(0);
        legend3.SetLineStyle(0);
        legend3.SetLineColorAlpha(0,0);
        legend3.Draw("same")

    print "data graph: ", data_d_array_y.tolist()
    print "data graph err up: ", data_d_array_eyh.tolist()
    print "data graph err dn: ", data_d_array_eyl.tolist()
    print "data hist : ", [h_data.GetBinContent(i+1) for i in range(h_data.GetNbinsX())]
    print "total bkg : ", [h_total_background.GetBinContent(i+1) for i in range(h_total_background.GetNbinsX())]

    gPad.RedrawAxis()


    plots.Print(outputplot)

    h_nonreso.Delete()
    h_vvreso.Delete()
    h_zjets.Delete()
    h_data.Delete() 


### Define function for processing of os command
def processCmd(cmd, quiet = 0):
    output = '\n'
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=STDOUT,bufsize=-1)
    for line in iter(p.stdout.readline, ''):
        output=output+str(line)
        print line,
    p.stdout.close()
    if p.wait() != 0:
        raise RuntimeError("%r failed, exit status: %d" % (cmd, p.returncode))
    if (not quiet):
        print 'Output:\n   ['+output+'] \n'
    return output

def Run():
    parser=parseOptions()
    (options,args) = parser.parse_args()

    inputfile = options.inputfile
    output = options.output
    observable=options.Observable
    #unblind=options.unblind

    #signal="xzz1000"
    signal=options.signal

    #input file
    file_in = TFile(inputfile)

    # start a general plot to store plots
    plots = TCanvas("plots","plots", 800, 800)
    plots.SetLogy()
    plots.SetBottomMargin(0.3)
    plots.SetRightMargin(0.05);

    # open the plots file
    outputplot = output+".pdf"
    plots.Print(outputplot+"[")
  
    # draw pull instead of ratio
    draw_pull=True
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_prefit', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_prefit', "mm", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_b', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_b', "mm", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_s', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_s', "mm", "SR", draw_pull)
    draw_pull=False
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_prefit', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_prefit', "mm", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_b', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_b', "mm", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_s', "ee", "SR", draw_pull)
    plotDataMC(file_in, outputplot, plots, observable, signal, 'shapes_fit_s', "mm", "SR", draw_pull)

    

    # close the plots file
    plots.Print(outputplot+"]")

if __name__ == "__main__":
    Run()
