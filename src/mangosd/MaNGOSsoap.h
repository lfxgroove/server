/**
 * MaNGOS is a full featured server for World of Warcraft, supporting
 * the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
 *
 * Copyright (C) 2005-2014  MaNGOS project <http://getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#ifndef _MANGOSSOAP_H
#define _MANGOSSOAP_H

#include "Common.h"
#include "World.h"
#include "AccountMgr.h"
#include "Log.h"

#include "soapH.h"
#include "soapStub.h"

#include <ace/Semaphore.h>
#include <ace/Task.h>

class MaNGOSsoapRunnable: public ACE_Based::Runnable
{
    public:
        MaNGOSsoapRunnable() { }
        void run() override;
        void setListenArguments(std::string host, uint16 port)
        {
            m_host = host;
            m_port = port;
        }
    private:
        std::string m_host;
        uint16 m_port;
};

class SOAPWorkingThread : public ACE_Task<ACE_MT_SYNCH>
{
    public:
        SOAPWorkingThread()
        { }

        virtual int svc(void) override
        {
            while (1)
            {
                ACE_Message_Block* mb = 0;
                if (this->getq(mb) == -1)
                {
                    ACE_DEBUG((LM_INFO,
                    ACE_TEXT("(%t) Shutting down\n")));
                    break;
                }

                // Process the message.
                process_message(mb);
            }

            return 0;
        }
    private:
        void process_message(ACE_Message_Block* mb);
};

class SOAPCommand
{
    public:
        SOAPCommand():
            pendingCommands(0, USYNC_THREAD, "pendingCommands")
        {
        }
        ~SOAPCommand()
        {
        }

        void appendToPrintBuffer(const char* msg)
        {
            m_printBuffer += msg;
        }

        ACE_Semaphore pendingCommands;

        void setCommandSuccess(bool val)
        {
            m_success = val;
        }
        bool hasCommandSucceeded()
        {
            return m_success;
        }

        static void print(void* callbackArg, const char* msg)
        {
            ((SOAPCommand*)callbackArg)->appendToPrintBuffer(msg);
        }

        static void commandFinished(void* callbackArg, bool success);

        bool m_success;
        std::string m_printBuffer;
};

#endif
