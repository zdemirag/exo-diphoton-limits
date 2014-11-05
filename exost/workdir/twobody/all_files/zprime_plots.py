#!/usr/bin/env python

################################################
#
# This script plots limits for the Z' dilepton
# analysis with CMS
#
#   - input data from ASCII files
#
# Usage:
#        ./plots.py --plot-number 1 -f pdf
# 1,2,3 - nominal limit plots
#
# Gena Kukartsev, 2010-2012
#
################################################

import ROOT
from plotter import *


suffix = '16feb2012v1'

plot_observed = False

prel = False
    
def zprime_plots(plot_format = 'png',
                 plot_number = [1]):
    #
    # Make plots with specified plot numbers
    #


    #------------------------------------------------------------>
    #1
    # dielectron
    # Bayesian limit
    #    

    if str(1) in plot_number:

        ROOT.gStyle.SetHatchesLineWidth(2)
        ROOT.gStyle.SetLineStyleString(11,"60 30");
        
        data = Data(4.98)
        
        _scale = 0.001
        _scale_theory = 1.0/970.0
        #_scale_exp = _scale*3335/3171 #/526809.0*603618.0
        _scale_exp = _scale

        #data.add_expected('exp', 'petyt_4700/expected_4700_scaled.ascii', 'Expected 95% C.L. limit',
        #data.add_expected('exp', 'full_2011/dielectron_exp_17jan2012v2.txt', 'Expected 95% C.L. limit',
        #data.add_expected('exp', 'full_2011/dielectron_exp_17jan2012v3.txt', 'Expected 95% C.L. limit',
        data.add_expected('exp', 'full_2011/dielectron_exp_16feb2012v1.txt', 'Expected 95% C.L. limit',
                          #_scale,
                          _scale_exp,
                             ROOT.kGreen, 0, 0,
                             ROOT.kYellow, 1.3, 8,
                             value_type = 'median',
                             error_type = 'quantile',
                             fill_style = 1002,
                             extra_scale_name = 'unit',
                             fill_color = ROOT.kGreen,
                             fill2_color = ROOT.kYellow,
                             smooth = None)

        data.add('SSM', 'theory/zprime_ssm_cteq6l1_masswindow.txt', 'Z\'_{SSM}',
                 1.0e+9*_scale_theory,
                 ROOT.kGreen-6, 1, 3,
                 ROOT.kGreen -6, 0.1, 8,
                 var_scale = 'cteq6l1_fit')

        data.add('Psi', 'theory/zprime_psi_cteq6l1_masswindow.txt', 'Z\'_{#Psi}',
                 1.0e+9*_scale_theory,
                 ROOT.kBlue, 1, 3,
                 ROOT.kBlue, 0.1, 8,
                 var_scale = 'cteq6l1_fit')
        
        data.add('RS0.1', 'theory/rsg01_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.1',
                 1.0e+9*_scale_theory,
                 ROOT.kBlue-1, 1, 1,
                 ROOT.kBlue-1, 0.1, 8,
                 var_scale = 'graviton',
                    #fill_style = 3013,
                 fill_style = 1002,
                 fill_color = ROOT.kBlue+2)

        data.add('RS0.05', 'theory/rsg005_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.05',
                 1.0e+9*_scale_theory,
                 ROOT.kGreen-6, 1, 1,
                 ROOT.kGreen -6, 0.1, 8,
                 var_scale = 'graviton',
                 fill_style = 1002)

        data.add('Stu02', 'theory/zprime_stu_ep020_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.02',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-10, 1, 1,
                 ROOT.kMagenta-10, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu03', 'theory/zprime_stu_ep030_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.03',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-7, 1, 1,
                 ROOT.kMagenta-7, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu04', 'theory/zprime_stu_ep040_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.04',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-3, 1, 1,
                 ROOT.kMagenta-3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu05', 'theory/zprime_stu_ep050_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.05',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta, 1, 1,
                 ROOT.kMagenta, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu06', 'theory/zprime_stu_ep060_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.06',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta+3, 1, 1,
                 ROOT.kMagenta+3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        #data.add_median('obs', 'full_2011/ee_obs_man_27feb2012v1.txt', '95% C.L. limit',
        data.add_median('obs', 'full_2011/ee_obs_27feb2012v2.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/ee_cteq_22feb2012v1.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/ee_grav_22feb2012v1.txt', '95% C.L. limit',
                        _scale,
                        1, 1, 3,
                        1, 0.8, 8)
                        
#        data.add_median('obs2', 'full_2011/ee_mass_cteq_low_5ifb.txt', '95% C.L. limit',
#                        _scale,
#                        2, 1, 3,
#                        2, 1, 8)

        makeMultiGraph(data, 'zprime_dielectron_5ifb.'+plot_format,
                       #300, 2500, 0.0000005, 0.00010,
                       300, 2500, 0.0000003, 0.00010,
                       #xlabel = "M [GeV]", ylabel = 'R_{#sigma} #upoint 10^{-4}',
                       xlabel = "M [GeV]", ylabel = 'R_{#sigma}',
                       xLegend = .63, yLegend = .28,
                       legendWidth = 0.45, legendHeight = 0.60,
                       fillStyle = 1002,
                       drawOption = 'APC3', letter = '',
                       toplegend = '#font[42]{ee}',
                       prel = prel,
                       #find_intersection = True,
                       #draw_limit_line = True
                       )



    #------------------------------------------------------------>
    #2
    # dimuon
    # Bayesian limit
    #    

    if str(2) in plot_number:

        ROOT.gStyle.SetHatchesLineWidth(2)
        ROOT.gStyle.SetLineStyleString(11,"60 30");
        
        data = Data(5.28)
        
        _scale = 0.001
        _scale_theory = 1.0/970.0
    
        #data.add_expected('exp', 'dimuon_exp_16jan2012v1.txt', 'Expected 95% C.L. limit',
        #data.add_expected('exp', 'full_2011/dimuon_exp_16jan2012v2.txt', 'Expected 95% C.L. limit',
        data.add_expected('exp', 'full_2011/dimuon_exp_16feb2012v1.txt', 'Expected 95% C.L. limit',
                         _scale,
                         ROOT.kGreen, 0, 0,
                         ROOT.kYellow, 1.3, 8,
                         value_type = 'median',
                         error_type = 'quantile',
                         fill_style = 1002,
                         extra_scale_name = 'unit',
                         fill_color = ROOT.kGreen,
                         fill2_color = ROOT.kYellow,
                         smooth = None)

        data.add('SSM', 'theory/zprime_ssm_cteq6l1_masswindow.txt', 'Z\'_{SSM}',
                    1.0e+9*_scale_theory,
                    ROOT.kGreen-6, 1, 3,
                    ROOT.kGreen -6, 0.1, 8,
                    var_scale = 'cteq6l1_fit')

        data.add('Psi', 'theory/zprime_psi_cteq6l1_masswindow.txt', 'Z\'_{#Psi}',
                    1.0e+9*_scale_theory,
                    ROOT.kBlue, 1, 3,
                    ROOT.kBlue, 0.1, 8,
                    var_scale = 'cteq6l1_fit')

        data.add('RS0.1', 'theory/rsg01_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.1',
                    1.0e+9*_scale_theory,
                    ROOT.kBlue-1, 1, 1,
                    ROOT.kBlue-1, 0.1, 8,
                    var_scale = 'graviton',
                    #fill_style = 3013,
                    fill_style = 1002,
                    fill_color = ROOT.kBlue+2)

        data.add('RS0.05', 'theory/rsg005_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.05',
                    1.0e+9*_scale_theory,
                    ROOT.kGreen-6, 1, 1,
                    ROOT.kGreen -6, 0.1, 8,
                    var_scale = 'graviton',
                    fill_style = 1002)

        data.add('Stu02', 'theory/zprime_stu_ep020_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.02',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-10, 1, 1,
                 ROOT.kMagenta-10, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu03', 'theory/zprime_stu_ep030_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.03',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-7, 1, 1,
                 ROOT.kMagenta-7, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu04', 'theory/zprime_stu_ep040_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.04',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-3, 1, 1,
                 ROOT.kMagenta-3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu05', 'theory/zprime_stu_ep050_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.05',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta, 1, 1,
                 ROOT.kMagenta, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu06', 'theory/zprime_stu_ep060_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.06',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta+3, 1, 1,
                 ROOT.kMagenta+3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        #data.add_median('obs', 'dimuon_obs_15jan2012v1.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/dimuon_obs_15jan2012v2.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/dimuon_obs_21feb2012v1.txt', '95% C.L. limit',
        data.add_median('obs', 'full_2011/mumu_obs_28feb2012v2.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/dimuon_mass_cteq_18jan2012v1.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/dimuon_mass_rs_18jan2012v1.txt', '95% C.L. limit',
                           _scale,
                           1, 1, 3,
                           1, 1, 8)

#        data.add_median('obs2', 'full_2011/mumu_mass_cteq_low_5ifb.txt', '95% C.L. limit',
#                        _scale,
#                        2, 1, 3,
#                        2, 1, 8)

        makeMultiGraph(data, 'zprime_dimuon_5ifb.'+plot_format,
                       #300, 2500, 0.005, 0.6,
                       #300, 2500, 0.0000005, 0.00006,
                       300, 2500, 0.0000003, 0.0001,
                       #xlabel = "M [GeV]", ylabel = 'R_{#sigma} #upoint 10^{-4}',
                       xlabel = "M [GeV]", ylabel = 'R_{#sigma}',
                       xLegend = .63, yLegend = .28,
                       legendWidth = 0.45, legendHeight = 0.60,
                       fillStyle = 1002,
                       drawOption = 'APC3', letter = '',
                       toplegend = '#font[42]{#mu^{+}#mu^{-}}',
                       prel = prel,
                       #find_intersection = True,
                       #draw_limit_line = True
                       )



    #------------------------------------------------------------>
    #3
    # dielectron + dimuon
    # Bayesian limit
    #    

    if str(3) in plot_number:

        ROOT.gStyle.SetHatchesLineWidth(2)
        ROOT.gStyle.SetLineStyleString(11,"60 30");
        
        data = Data()
        
        _scale = 0.001
        _scale_theory = 1.0/970.0
    
        data.add_expected('exp', 'full_2011/dilepton_exp_16feb2012v1.txt', 'Expected 95% C.L. limit',
                             _scale,
                             ROOT.kGreen, 0, 0,
                             ROOT.kYellow, 1.3, 8,
                             value_type = 'median',
                             error_type = 'quantile',
                             fill_style = 1002,
                             extra_scale_name = 'unit',
                             fill_color = ROOT.kGreen,
                             fill2_color = ROOT.kYellow,
                             smooth = None)

        data.add('SSM', 'theory/zprime_ssm_cteq6l1_masswindow.txt', 'Z\'_{SSM}',
                    1.0e+9*_scale_theory,
                    ROOT.kGreen-6, 1, 3,
                    ROOT.kGreen -6, 0.1, 8,
                    var_scale = 'cteq6l1_fit')

        data.add('Psi', 'theory/zprime_psi_cteq6l1_masswindow.txt', 'Z\'_{#Psi}',
                    1.0e+9*_scale_theory,
                    ROOT.kBlue, 1, 3,
                    ROOT.kBlue, 0.1, 8,
                    var_scale = 'cteq6l1_fit')

        data.add('RS0.1', 'theory/rsg01_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.1',
                    1.0e+9*_scale_theory,
                    ROOT.kBlue-1, 1, 1,
                    ROOT.kBlue-1, 0.1, 8,
                    var_scale = 'graviton',
                    #fill_style = 3013,
                    fill_style = 1002,
                    fill_color = ROOT.kBlue+2)

        data.add('RS0.05', 'theory/rsg005_cteq6l1_masswindow.txt', 'G_{#font[132]{KK}} k/#bar{M_{#font[132]{Pl}}}=0.05',
                    1.0e+9*_scale_theory,
                    ROOT.kGreen-6, 1, 1,
                    ROOT.kGreen -6, 0.1, 8,
                    var_scale = 'graviton',
                    fill_style = 1002)

        data.add('Stu02', 'theory/zprime_stu_ep020_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.02',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-10, 1, 1,
                 ROOT.kMagenta-10, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu03', 'theory/zprime_stu_ep030_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.03',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-7, 1, 1,
                 ROOT.kMagenta-7, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu04', 'theory/zprime_stu_ep040_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.04',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta-3, 1, 1,
                 ROOT.kMagenta-3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu05', 'theory/zprime_stu_ep050_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.05',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta, 1, 1,
                 ROOT.kMagenta, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add('Stu06', 'theory/zprime_stu_ep060_cteq6l1_masswindow.txt', 'Z\'_{St} #epsilon = 0.06',
                 1.0e+9*_scale_theory,
                 ROOT.kMagenta+3, 1, 1,
                 ROOT.kMagenta+3, 0.1, 8,
                 var_scale = 'cteq6l1_fit') 

        data.add_median('obs', 'full_2011/ll_obs_27feb2012v2.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/ll_cteq_25feb2012v1.txt', '95% C.L. limit',
        #data.add_median('obs', 'full_2011/ll_grav_25feb2012v1.txt', '95% C.L. limit',
                           _scale,
                           1, 1, 3,
                           1, 1, 8)

#        data.add_median('obs2', 'full_2011/ll_mass_cteq_low_5ifb.txt', '95% C.L. limit',
#                           _scale,
#                           2, 1, 3,
#                           2, 1, 8)

        makeMultiGraph(data, 'zprime_dilepton_5ifb.'+plot_format,
                       300, 2500, 0.0000003, 0.0001,
                       xlabel = "M [GeV]", ylabel = 'R_{#sigma}',
                       xLegend = .63, yLegend = .28,
                       legendWidth = 0.45, legendHeight = 0.60,
                       fillStyle = 1002,
                       drawOption = 'APC3', letter = '',
                       toplegend = '#font[42]{ee(4.98fb^{-1})+#mu^{+}#mu^{-}(5.28fb^{-1})}',
                       prel = prel,
                       #find_intersection = True,
                       #draw_limit_line = True
                       )




