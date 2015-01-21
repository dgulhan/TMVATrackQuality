quick setup steps

run TMVAClassicifaction("",0)

I have the TMVAClassificationApplication set up to run on a separate unmerged
file, so its different than the one we trained on (but the same MC
production).

Current to-do's:
TMVAClassificationApplication.C is almost finished, just need to get the TTree variables pointing to the same place as the inputs to the TMVA reader
Should be almost done, but I forgot that all the trackTree variables are arrays indexed by nTrk, so we need to fix that.  The output should be some TMVA
"score" that we can use to cut on to get a desired signal/background ratio.
