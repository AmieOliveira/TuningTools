#ifndef TUNINGTOOLS_TRAINING_PATTERNREC_H
#define TUNINGTOOLS_TRAINING_PATTERNREC_H

#include <vector>

#include "TuningTools/system/defines.h"
#include "TuningTools/training/Training.h"

#define MIN_DELTA_FIT         0.1

class PatternRecognition : public Training
{
  protected:
    const REAL **inTrnList;
    const REAL **inValList;
    const REAL **inTstList;
    const REAL **targList;
    REAL **epochValOutputs;
    REAL **epochTstOutputs;
    unsigned numPatterns;
    unsigned inputSize;
    unsigned outputSize;

    bool hasTstData;

    // Multi stop best values holder
    REAL bestGoalSP;  // Best SP founded
    REAL bestGoalDet; // Best Detection founded using stop by fa
    REAL bestGoalFa;  // Best False Alarm founded using stop by det
    // This will be enable when trainGoal is: SP_STOP or MULTI_STOP
    bool useSP;
    // Setter values to fitted the operation point into the roc
    REAL goalDet;
    REAL goalFa;
    // Fitted values
    REAL detFitted;
    REAL faFitted;
    // Delta values: (goal-value)
    REAL deltaDet;
    REAL deltaFa;


    REAL signalWeight;
    REAL noiseWeight;
    std::vector<DataManager*> dmTrn;
    unsigned *numValEvents;
    unsigned *numTstEvents;
    // This will be used to select the validation criteria
    TrainGoal trainGoal;

    void allocateDataset( std::vector<Ndarray<REAL,2>*> dataSet, 
                          const bool forTrain, 
                          const REAL **&inList, REAL **&out, 
                          unsigned *&nEv);

    void deallocateDataset(const bool forTrain, 
                           const REAL **&inList, 
                           REAL **&out,
                           unsigned *&nEv);
    
    void getNetworkErrors(const REAL **inList, 
                          const unsigned *nEvents, 
                          REAL **epochOutputs, 
                          REAL &mseRet, 
                          REAL &spRet, 
                          REAL &detRet, 
                          REAL &faDet);

    /// Helper function
    void isBestGoal( const REAL currError, 
                     REAL &bestGoalRet, 
                     ValResult &isBestRet )
    {
      if (currError > bestGoalRet)
      {
        bestGoalRet = currError;
        isBestRet = BETTER;
      }
      else if (currError < bestGoalRet) isBestRet = WORSE;
      else isBestRet = EQUAL;
    };

  public:

    PatternRecognition(TuningTool::Backpropagation *net, 
                       std::vector< Ndarray<REAL,2>* > inTrn, 
                       std::vector< Ndarray<REAL,2>* > inVal, 
                       std::vector< Ndarray<REAL,2>* > inTst, 
                       TrainGoal  mode, const unsigned bSize,
                       const REAL signalWeigh = 1.0, 
                       const REAL noiseWeight = 1.0, 
                       MSG::Level msglevel = MSG::INFO);

    virtual ~PatternRecognition();

    /**
     * @brief Calculates the SP product.
     *
     * Calculates the SP product. This method will run through the dynamic
     * range of the outputs, calculating the SP product in each lambda value.
     * Returning, at the end, the maximum SP product obtained.
     *
     * If trainGoal is SP_STOP, det and fa will be the detection and false alarm
     * over the max sp point founded. But if the trains mode is MULTI_STOP, these
     * values will be the detection founded to a False alarm fitted and a false
     * alarm to a detection fitted.
     * The fitted values must be passed using the function:
     *   void setReferences(REAL det, REAL fa)
     *
     * @return The maximum SP value obtained. You can hold the signal and noise
     *         effic pass by reference.
     **/
    virtual REAL sp(const unsigned *nEvents, REAL **epochOutputs, REAL &det, REAL &fa );

    virtual void tstNetwork(REAL &mseTst, REAL &spTst, REAL &detTst, REAL &faTst)
    {
      MSG_DEBUG("Starting testing process for an epoch.");
      getNetworkErrors(inTstList, numTstEvents, epochTstOutputs, mseTst, spTst, detTst, faTst);
    }


    /// Applies the validating set of each pattern for the network's validation.
    /**
     * This method takes the one or more pattern's validating events (input and
     * targets) and presents them to the network. At the end, the mean training
     * error is returned. Since it is a validating function, the network is not
     * modified, and no updating weights values are calculated. This method
     * only presents the validating sets and calculates the mean validating
     * error obtained.
     * @param[in] net the network class that the events will be presented to.
     *                The internal parameters of this class are not modified
     *                inside this method, since it is only a network validating
     *                process.
     * @return The mean validating error obtained after the entire training set
     *         is presented to the network.
     **/
    virtual void valNetwork(REAL &mseVal, 
                            REAL &spVal, 
                            REAL &detVal,
                            REAL &faVal)
    {
      MSG_DEBUG("Starting validation process for an epoch.");
      getNetworkErrors(inValList, 
                       numValEvents, 
                       epochValOutputs, 
                       mseVal, 
                       spVal, 
                       detVal, 
                       faVal);
    }



    /**
     * @brief Applies the training set of each pattern for the network's training.
     *
     * This method takes the one or more patterns training events (input and
     * targets) and presents them to the network, calculating the new mean (if
     * batch training is being used) update values after each input-output pair
     * of each individual pattern is presented. At the end, the mean training
     * error is returned.  @param[in] net the network class that the events
     * will be presented to. At the end, this class is modificated, as it will
     * contain the mean values of \f$\Delta w\f$ and \f$\Delta b\f$ obtained
     * after the entire training set has been presented, but the weights are
     * not updated at the end of this function. To actually update the weights,
     * the user must call the proper class's method for that.
     *
     * @return The mean training error obtained after the entire training of each
     *         pattern set is presented to the network.
     **/
    virtual REAL trainNetwork();

    virtual void showInfo(const unsigned nEpochs) const;

    virtual void isBestNetwork(const REAL currMSEError, const REAL currSPError, const REAL currDetError,
                               const REAL currFaError,  ValResult &isBestMSE, ValResult &isBestSP,
                               ValResult &isBestDet,    ValResult &isBestFa );
    
    virtual void showTrainingStatus(const unsigned epoch, const REAL mseTrn, const REAL mseVal, const REAL spVal, 
                                      const int stopsOn);
    

    virtual void showTrainingStatus(const unsigned epoch, const REAL mseTrn, const REAL mseVal, const REAL spVal, 
                                    const REAL mseTst, const REAL spTst, const int stopsOn);




    void setReferences( const REAL det, const REAL fa )
    {
      goalDet = det;
      goalFa  = fa;
      MSG_INFO("Setting references: DET = " << det << " and FA = " << fa);
    }

    void setDeltaDet( REAL delta = MIN_DELTA_FIT ){
      deltaDet = delta;
    }

    void setDeltaFa( REAL delta = MIN_DELTA_FIT ){
      deltaFa = delta;
    }

    void retrieveFittedValues( REAL &det, REAL &fa, REAL &dDet, REAL &dFa)
    {
      det = detFitted;  fa = faFitted;  dDet = deltaDet;  dFa = deltaFa;
    }

    virtual void resetBestGoal(){
      Training::resetBestGoal();
      bestGoalSP = bestGoalDet = bestGoalFa = 0.0;
    }



};

#endif
