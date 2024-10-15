/* MIT License
 *
 * Copyright (c) 2024 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  System state: Init
 * @author Andreas Merkle <web@blue-andi.de>
 *
 * @addtogroup sys_states
 *
 * @{
 */

#ifndef INITSTATE_H
#define INITSTATE_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <WString.h>
#include <StateMachine.hpp>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * Initialization state:
 * - Initializes the board.
 * - Check for user button press during start up.
 */
class InitState : public AbstractState
{
public:

    /**
     * Get state instance.
     *
     * @return State instance
     */
    static InitState& getInstance()
    {
        static InitState instance; /* singleton idiom to force initialization in the first usage. */

        return instance;
    }

    /**
     * The entry is called once, a state is entered.
     *
     * @param[in] sm    Responsible state machine
     */
    void entry(StateMachine& sm) final;

    /**
     * The process routine is called cyclic, as long as the state is active.
     *
     * @param[in] sm    Responsible state machine
     */
    void process(StateMachine& sm) final;

    /**
     * The exit is called once, a state will be left.
     *
     * @param[in] sm    Responsible state machine
     */
    void exit(StateMachine& sm) final;

private:

    /**
     * Constructs the state.
     */
    InitState()
    {
    }

    /**
     * Destroys the state.
     */
    ~InitState()
    {
    }

    InitState(const InitState& state);
    InitState& operator=(const InitState& state);

    /**
     * Show startup information on the serial interface.
     */
    void showStartupInfoOnSerial(void);

    /**
     * Checks whether the filesystem content is compatible to the IVTRego6xxControl version.
     * 
     * @return If filesystem content is compatible, it will return true otherwise false.
     */
    bool isFsCompatible();

    /**
     * Mount the filesystem.
     * 
     * @return If successful mounted, it will return true otherwise false.
     */
    bool mountFilesystem();

    /**
     * Get device unique ID as string.
     * The device unique ID is derived from factory programmed wifi MAC address.
     * 
     * @param[out] deviceUniqueId   The device unique id.
     */
    void getDeviceUniqueId(String& deviceUniqueId);
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* INITSTATE_H */

/** @} */