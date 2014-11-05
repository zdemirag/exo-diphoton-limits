# This is an example of a RooStats interval calculation
# for a counting experiment
#
# PyROOT approach

#
# RooStats exercise at CMSDAS-2011:
#   Fermilab, January 25-29, 2011
#   author: Gena Kukartsev
#   inspired by official RooStats tutorials,
#   see http://root.cern.ch/root/html/tutorials/roostats/
#
#   date: January 25, 2011
#

from ROOT import TCanvas
from ROOT import RooWorkspace
from ROOT import RooArgList
from ROOT import RooDataSet
from ROOT import RooPlot
from ROOT import RooStats


#
# this function implements the interval calculation for a counting experiment
#
 
# make model
wspace = RooWorkspace("myWS")
wspace . factory("Uniform::model_pdf_1(x[0,1])")
wspace . factory("Uniform::model_pdf_2(x)")
wspace . factory("SUM::model_pdf(s[0,15]*model_pdf_1,b[1,0,2]*model_pdf_2)")
wspace . factory("Lognormal::likelihood_b(b,1,3)")
wspace . factory("PROD::model_likelihood(model_pdf, likelihood_b)")
wspace . factory("Uniform::prior_pdf(s)")
  
# define observables
wspace . defineSet("observables","x")

# define parameters of interest
wspace . defineSet("poi","s")

# define nuisance parameters
wspace . defineSet("nuisance_parameters","b")

# load data
observables = RooArgList( wspace.set("observables") )
data = RooDataSet.read("counting_data_3.ascii", observables)
data . SetName("data")
getattr(wspace, 'import')(data)

# model config
modelConfig = RooStats.ModelConfig("counting_model_config")
modelConfig . SetWorkspace(wspace)
modelConfig . SetPdf(wspace.pdf("model_likelihood"))
modelConfig . SetPriorPdf(wspace.pdf("prior_pdf"))
modelConfig . SetParametersOfInterest(wspace.set("poi"))
modelConfig . SetNuisanceParameters(wspace.set("nuisance_parameters"))
getattr(wspace, 'import')(modelConfig, "counting_model_config")

# Bayesian Calculator
bc = RooStats.BayesianCalculator(data, modelConfig)
bc.SetName("exostBayes")
getattr(wspace, 'import')(bc)

# inspect workspace
wspace . Print()

# save workspace to file
wspace . writeToFile("myWS.root")

# run Bayesian calculation
#bc.SetConfidenceLevel(0.95)
#bInt = bc.GetInterval()

# make plots
#c1 = TCanvas("c1")
#bplot = bc.GetPosteriorPlot()
#bplot . Draw()
#c1 . SaveAs("posterior_pdf.png")

# query interval
#print "Bayesian interval on s = [",
#print bInt.LowerLimit( ), ",",
#print bInt.UpperLimit( ), "]"


# Profile Likelihood Calculator
plc = RooStats.ProfileLikelihoodCalculator(data, modelConfig)

# run Profile Likelihood Calculator
plc.SetConfidenceLevel(0.95)
plInt = plc.GetInterval();

# make plots
c1 = TCanvas("c1")
lrplot = RooStats.LikelihoodIntervalPlot(plInt)
lrplot.Draw()
c1 . SaveAs("likelihood_interval.png")

# query interval
plc_lower = plInt.LowerLimit( wspace.var("s") )
plc_upper = plInt.UpperLimit( wspace.var("s") )
print "Profile Likelihood interval on s = [",
print plc_lower, ",",
print plc_upper, "]"


