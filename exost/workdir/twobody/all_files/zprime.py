import ROOT

_file = ROOT.TFile('ws_dimuon_ratio.root', 'r')
ws = _file.Get('myWS')
ws.Print()

model = ws.pdf('model')
data = ws.data('data')

ws.var('mass').setRange(600.0, 1500.0)
ws.var('peak').setVal(1000.0)

model.fitTo(data,ROOT.RooFit.Save())

ws.var('glob_nsig').Print()
ws.var('glob_nbkg').Print()
