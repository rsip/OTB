//-------------------------------------------------------------------------
//
// This code was taken from Open Scene Graph and incorporated from into
// OSSIM.
//
//-------------------------------------------------------------------------
// $Id: ossimApplicationUsage.h,v 1.3 2005/08/18 14:31:44 gpotts Exp $
#ifndef ossimApplicationUsage_HEADER
#define ossimApplicationUsage_HEADER 1

#include <base/common/ossimConstants.h>
#include <base/data_types/ossimString.h>
#include <map>
#include <string>
#include <iostream>

class OSSIMDLLEXPORT ossimApplicationUsage
{
    public:
        
        static ossimApplicationUsage* instance();

        ossimApplicationUsage() {}

        ossimApplicationUsage(const ossimString& commandLineUsage);

        typedef std::map<ossimString,ossimString> UsageMap;
        

        void setApplicationName(const ossimString& name);
        const ossimString& getApplicationName() const;

        void setDescription(const ossimString& desc);
        const ossimString& getDescription() const;

        enum Type
        {
            OSSIM_COMMAND_LINE_OPTION    = 0x1,
            OSSIM_ENVIRONMENTAL_VARIABLE = 0x2
        };
        
        void addUsageExplanation(Type type,const ossimString& option,const ossimString& explanation);
        
        void setCommandLineUsage(const ossimString& explanation);

        const ossimString& getCommandLineUsage() const;


        void addCommandLineOption(const ossimString& option,const ossimString& explanation);
        
        const UsageMap& getCommandLineOptions() const;


        void addEnvironmentalVariable(const ossimString& option,const ossimString& explanation);
        
        const UsageMap& getEnvironmentalVariables() const;

        void getFormatedString(ossimString& str, const UsageMap& um,unsigned int widthOfOutput=80);

        void write(std::ostream& output,const UsageMap& um,unsigned int widthOfOutput=80);
        
        void write(std::ostream& output,unsigned int type=OSSIM_COMMAND_LINE_OPTION|OSSIM_ENVIRONMENTAL_VARIABLE, unsigned int widthOfOutput=80);

    protected:
    
        ossimString theApplicationName;
        ossimString theDescription;
        ossimString theCommandLineUsage;
        UsageMap    theCommandLineOptions;
        UsageMap    theEnvironmentalVariables;
};

class ApplicationUsageProxy
{   
    public:

        /** register an explanation of commandline/evironmentalvaraible/keyboard mouse usage.*/
        ApplicationUsageProxy(ossimApplicationUsage::Type type,const ossimString& option,const ossimString& explanation)
        {
            ossimApplicationUsage::instance()->addUsageExplanation(type,option,explanation);
        }
};


#endif
