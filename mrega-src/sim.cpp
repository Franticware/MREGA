#include "sim.h"
#include <cmath>
#include <cstdio>
#include "anytk.h"

const Real deltaT = 1.0/20.0; // 20 fps -> 50 ms

const Real carLength = 4; // m
const Real carWidth = 1.6;
const Real carMass = 1000; // kg
const Real carAcc = 0.6; // m * s ^ -2
// F = m * a
const Real ovechkin1 = 10;
const Real carForce = carMass * carAcc * ovechkin1;

const Real carGap = 0.1; // gap between cars after collision

const Real colRatio = 0.6;

int state = 0;
int stateTick = 0;

int simLights = 0;

int simExitToMenu = 0;

#define LAP_WIN 4 // by entering this lap, player wins

void resetState()
{
    state = 0;
    stateTick = 0;
    simLights = 1;

    simExitToMenu = 0;
}

int getExitToMenu()
{
    return simExitToMenu;
}

int getLights()
{
    return simLights;
}


//D = Cd * A * .5 * r * V^2

Real brakeForce = carForce * 1.5;

Real airDrag(Real velocity, bool road)
{
    const Real airDragMultiplier = road ? 9 : 20;
    return velocity*velocity*airDragMultiplier;
}

Real rollDrag(bool road)
{
    return road ? carForce * 0.1 : carForce * 0.8;
}

const Real tableCos[91] = {
    1.000000,0.999848,0.999391,0.998630,0.997564,0.996195,0.994522,0.992546,0.990268,0.987688,0.984808,0.981627,0.978148,0.974370,0.970296,0.965926,0.961262,0.956305,0.951057,0.945519,0.939693,0.933580,0.927184,0.920505,0.913545,0.906308,0.898794,0.891007,0.882948,0.874620,0.866025,0.857167,0.848048,0.838671,0.829038,0.819152,0.809017,0.798636,0.788011,0.777146,0.766044,0.754710,0.743145,0.731354,0.719340,0.707107,0.694658,0.681998,0.669131,0.656059,0.642788,0.629320,0.615661,0.601815,0.587785,0.573576,0.559193,0.544639,0.529919,0.515038,0.500000,0.484810,0.469472,0.453990,0.438371,0.422618,0.406737,0.390731,0.374607,0.358368,0.342020,0.325568,0.309017,0.292372,0.275637,0.258819,0.241922,0.224951,0.207912,0.190809,0.173648,0.156434,0.139173,0.121869,0.104528,0.087156,0.069756,0.052336,0.034899,0.017452,0.000000,
    };

const Real tableSin[91] = {
    0.000000,0.017452,0.034899,0.052336,0.069756,0.087156,0.104528,0.121869,0.139173,0.156434,0.173648,0.190809,0.207912,0.224951,0.241922,0.258819,0.275637,0.292372,0.309017,0.325568,0.342020,0.358368,0.374607,0.390731,0.406737,0.422618,0.438371,0.453990,0.469472,0.484810,0.500000,0.515038,0.529919,0.544639,0.559193,0.573576,0.587785,0.601815,0.615661,0.629320,0.642788,0.656059,0.669131,0.681998,0.694658,0.707107,0.719340,0.731354,0.743145,0.754710,0.766044,0.777146,0.788011,0.798636,0.809017,0.819152,0.829038,0.838671,0.848048,0.857167,0.866025,0.874620,0.882948,0.891007,0.898794,0.906308,0.913545,0.920505,0.927184,0.933580,0.939693,0.945519,0.951057,0.956305,0.961262,0.965926,0.970296,0.974370,0.978148,0.981627,0.984808,0.987688,0.990268,0.992546,0.994522,0.996195,0.997564,0.998630,0.999391,0.999848,1.000000,
    };

void adjustX(Real& xpos, Real zposPrev, Real zpos, uint32_t* track, uint32_t trackSize)
{
    if (zposPrev != zpos)
    {
        /*printf("%f %f\n", zposPrev, zpos);
        fflush(stdout);*/

        Real xoff = 0;
        Real xoffmul = -0.6;
        //Real xoffmul = -1.0;

        Real zposLow;
        Real zposHigh;

        if (zposPrev < zpos)
        {
            zposLow = zposPrev;
            zposHigh = zpos;
        }
        else
        {
            zposHigh = zposPrev;
            zposLow = zpos;
            xoffmul *= -1;
        }

        int zposLowI = zposLow + trackSize;
        int zposHighI = zposHigh + trackSize;

        int zi;

        for (zi = zposLowI+1; zi <= zposHighI; ++zi)
        {
            if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0x0100)
            {
                xoff += 1;
            }
            else if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0xff00)
            {
                xoff -= 1;
            }
        }

        Real zposLowFrag = zposLow - std::floor(zposLow);
        Real zposHighFrag = zposHigh - std::floor(zposHigh);


        zi = zposLowI;

        if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0x0100)
        {
            xoff += (1.0-zposLowFrag);
        }
        else if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0xff00)
        {
            xoff -= (1.0-zposLowFrag);
        }

        zi = zposHighI;

        if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0x0100)
        {
            xoff -= (1.0-zposHighFrag);
        }
        else if ((track[(zi + trackSize) % trackSize] & 0xff00) == 0xff00)
        {
            xoff += (1.0-zposHighFrag);
        }

        /*if (xoff != 0)
        {
            printf("%f\n", xoff);
            fflush(stdout);
        }*/
        xpos += xoff * xoffmul;
    }

}

const int angleStep = 3;
const int angleMax = 45;


void simStepTurn(Player& player, int left, int right)
{
    bool playerATurning = false;
    if (left)
    {
        player.angle += angleStep;
        playerATurning = true;
    }
    if (right)
    {
        player.angle -= angleStep;
        playerATurning = true;
    }

    if (!playerATurning)
    {
        if (player.angle < 0)
        {
            player.angle += angleStep;
        }
        else if (player.angle > 0)
        {
            player.angle -= angleStep;
        }
    }

    if (player.angle > angleMax)
        player.angle = angleMax;
    if (player.angle < -angleMax)
        player.angle = -angleMax;

}



void simStepAdjustEnd(Player& player, Real prevZ, uint32_t* track, uint32_t trackSize)
{
    if (player.z < 0)
    {
        prevZ += trackSize;
        player.z += trackSize;
    }

    if (player.z >= trackSize)
    {
        prevZ -= trackSize;
        player.z -= trackSize;
    }

    if (prevZ + carLength < trackSize && player.z + carLength >= trackSize)
    {
        ++player.lap;
        if (player.lap == LAP_WIN)
        {
            player.finished = 1;
        }
    }

    adjustX(player.x, prevZ, player.z, track, trackSize);
}

void advancePos(Player& player)
{
    int absAngle = player.angle;
    if (absAngle < 0) absAngle = -absAngle;

    player.z += player.speed * deltaT * tableCos[absAngle];


    Real speedSideways = player.speed * tableSin[absAngle];

    const Real speedSidewaysLimit = 15.0/3.6;

    if (speedSideways > speedSidewaysLimit)
        speedSideways = speedSidewaysLimit;


    Real sidewaysRatio = speedSideways / speedSidewaysLimit;

    player.sprA = 0;

    if (absAngle > 10)
    {
        player.sprA = 1;
    }

    if (absAngle > 20)
    {
        player.sprA = 2;
    }

    if (sidewaysRatio < 0.7 && player.sprA == 2)
    {
        player.sprA = 1;
    }
    if (sidewaysRatio < 0.3 && player.sprA >= 1)
    {
        player.sprA = 0;
    }


    if (player.angle >= 0)
    {
        player.x -= speedSideways * deltaT;
        player.sprA = -player.sprA;
    }
    else
    {
        player.x += speedSideways * deltaT;
    }

}

const Real roadThr = 3.5;

void updateSpeed(Player& player, bool acl, bool brk)
{
    Real rfB = 0; // resultingForce

    if (acl)
    {
        rfB += carForce;
    }
    if (brk)
    {
        rfB -= brakeForce;
    }

    bool road = player.x > -roadThr && player.x < roadThr;

    rfB -= airDrag(player.speed, road);

    rfB -= rollDrag(road);


    // f = m * a
    Real accB = rfB / carMass;

    player.speed += accB * deltaT;

    if (player.speed < 0)
        player.speed = 0;

}

// 3.5 grass threshold
// 20 arrow threshold


uint8_t crashSpeedToVol(Real speed)
{
    const Real minSpeed = 4;
    const Real maxSpeed = 12;

    //const int8_t minVol = 32;
    const int8_t minVol = 40;
    const int8_t maxVol = 63;

    const Real minVolReal = minVol;
    const Real maxVolReal = maxVol;

    Real speedRatio = (speed - minSpeed)/(maxSpeed - minSpeed);
    Real volReal = minVolReal + (maxVolReal - minVolReal) * speedRatio;
    int32_t volInt = std::floor(volReal);
    if (volInt < minVol) volInt = minVol;
    else if (volInt > maxVol) volInt = maxVol;
    return (uint8_t)volInt;
}

void collideZ(Player& player0, Real pz0, Player& player1, Real pz1, uint32_t trackSize)
{
    (void)(pz1);
    Real xDiff = player1.x - player0.x;
    if (xDiff < 0)
        xDiff = -xDiff;
    if (xDiff <= carWidth)
    {
        bool isCol = false;
        Real addZ = 0;
        if (pz0 + carLength < player1.z && player0.z + carLength >= player1.z)
        {
            isCol = true;
            addZ = player0.z + carLength - player1.z + carGap;
        }
        else if (pz0 + carLength < player1.z + trackSize && player0.z + carLength >= player1.z + trackSize)
        {
            isCol = true;
            addZ = player0.z + carLength - (player1.z + trackSize) + carGap;
        }

        if (isCol)
        {
            player1.z += addZ;

            Real speedDiff = player0.speed - player1.speed;

            // collision sound generation - volume based on speed difference
            snd_crash(crashSpeedToVol(speedDiff));

#ifdef USESDL
            /*printf("speedDiff %f\n", speedDiff);
            fflush(stdout);*/
#endif

            Real tmp = player1.speed;
            player1.speed = player0.speed * colRatio;
            player0.speed = tmp * colRatio;

        }
    }
}


bool isZClose(const Player& player0, const Player& player1, uint32_t trackSize)
{
    Real zDiff = player0.z - player1.z;
    if (zDiff < 0) zDiff = -zDiff;
    Real zDiff2 = player0.z + trackSize - player1.z;
    if (zDiff2 < 0) zDiff2 = -zDiff2;
    Real zDiff3 = player0.z - trackSize - player1.z;
    if (zDiff3 < 0) zDiff3 = -zDiff3;
    if (zDiff2 < zDiff)
        zDiff = zDiff2;
    if (zDiff3 < zDiff)
        zDiff = zDiff3;
    return (zDiff <= carLength);
}


void collideX(Player& player0, Real px0, Player& player1, Real px1)
{
    if (px1 - px0 > carWidth && player1.x - player0.x <= carWidth)
    {
        Real addX = carWidth - (player1.x - player0.x) + carGap;

        player1.x += addX * 0.5;
        player0.x -= addX * 0.5;
    }
}

void collideX2(Player& player0, Real px0, Player& player1, Real px1)
{
    (void)(px0);
    (void)(px1);
    if (player1.x - player0.x <= carWidth && player1.x - player0.x >= 0)
    {
        Real addX = carWidth - (player1.x - player0.x) + carGap;

        player1.x += addX * 0.5;
        player0.x -= addX * 0.5;
    }
}

void collide(Player& playerA, Real prevAx, Real prevAz, Player& playerB, Real prevBx, Real prevBz, uint32_t trackSize)
{
    collideZ(playerA, prevAz, playerB, prevBz, trackSize);
    collideZ(playerB, prevBz, playerA, prevAz, trackSize);

    if (isZClose(playerA, playerB, trackSize))
    {
/*
#ifdef USESDL
        printf("zClose\n");
        fflush(stdout);
#endif
*/

        collideX(playerA, prevAx, playerB, prevBx);
        collideX(playerB, prevBx, playerA, prevAx);

        collideX2(playerA, prevAx, playerB, prevBx);
        collideX2(playerB, prevBx, playerA, prevAx);
    }
}

/*
---- State 0
1 s
---- 1 red
1 s
---- 2 red
1 s
---- State 1, 1 green
3 s
---- no lights

...

---- State 2 (finish) - no controls
2 s
---- brakes
5 s
---- Exit to menu

*/


int speed2fnum(Real speed)
{
    const uint16_t speedx10_2fnum[400] = {369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,369,370,379,389,398,408,417,427,436,446,455,465,474,484,493,503,512,522,531,540,550,559,569,578,588,597,607,616,626,635,645,654,664,673,683,692,702,711,721,730,740,749,759,768,778,787,398,403,408,412,417,422,427,431,436,441,446,450,455,460,465,469,474,479,484,488,493,498,503,507,512,517,522,526,531,536,540,545,550,555,559,564,569,574,578,583,588,593,597,602,607,612,616,621,626,631,635,640,645,650,654,659,664,669,673,678,683,688,692,697,702,707,711,716,721,726,730,735,740,745,749,754,759,764,768,773,778,783,787,528,531,534,537,540,544,547,550,553,556,559,563,566,569,572,575,578,582,585,588,591,594,597,601,604,607,610,613,616,620,623,626,629,632,635,639,642,645,648,651,654,658,661,664,667,670,673,677,680,683,686,689,692,696,699,702,705,708,711,714,718,721,724,727,730,733,737,740,743,746,749,752,756,759,762,765,768,771,775,778,781,784,787,569,571,574,576,578,580,583,585,587,589,592,594,596,599,601,603,605,608,610,612,615,617,619,621,624,626,628,630,633,635,637,640,642,644,646,649,651,653,656,658,660,662,665,667,669,671,674,676,678,681,683,685,687,690,692,694,697,699,701,703,706,708,710,712,715,717,719,722,724,726,728,731,733,735,738,740,742,744,747,749,751,753,756,758,760,763,765,767,769,772,774,776,779,781,783,785,788,790,792,794,797,799,801,804,806,808,810,813,815,817,820,822,824,826,829,831,833,835,838,840,842,845,847,849,851,854,856,858,861,863,865,867,870,872,874,876,879,881,883,886,888,890,892,895,897,899,902,904,906,908,};

    int i = std::floor(speed * 10);
    if (i < 0) i = 0;
    if (i >= 400) i = 399;
    return speedx10_2fnum[i];
}




void simStep(Player& playerA, Player& playerB, uint32_t* track, uint32_t trackSize)
{
    if (state == 0 && stateTick == 20)
    {
        simLights = 2; // 1 red
        snd_beepLights(false);
    }
    if (state == 0 && stateTick == 40)
    {
        simLights = 3; // 2 red
        snd_beepLights(false);
    }
    if (state == 0 && stateTick == 60)
    {
        simLights = 4; // 1 green
        snd_beepLights(true);
        stateTick = 0;
        state = 1;
    }
    if (state == 1 && stateTick == 40)
    {
        simLights = 0;
    }
    if (state == 2 && stateTick == 140)
    {
        simExitToMenu = 1;
    }

    Real playerApz = playerA.z;
    Real playerBpz = playerB.z;

    Real playerApx = playerA.x;
    Real playerBpx = playerB.x;

    simStepTurn(playerA, getKeyState(K_A), getKeyState(K_D));
    simStepTurn(playerB, getKeyState(K_LEFT), getKeyState(K_RIGHT));

    if (state == 0)
    {
        updateSpeed(playerA, false, false);
        updateSpeed(playerB, false, false);

    }
    else if (state == 1)
    {
        updateSpeed(playerA, getKeyState(K_W) || getKeyState(K_LSHIFT), getKeyState(K_S) || getKeyState(K_LCTRL));
        updateSpeed(playerB, getKeyState(K_UP) || getKeyState(K_RSHIFT), getKeyState(K_DOWN) || getKeyState(K_RCTRL));
    }
    else if (state == 2)
    {
        if (stateTick < 20)
        {
            updateSpeed(playerA, false, false);
            updateSpeed(playerB, false, false);
        }
        else
        {
            updateSpeed(playerA, false, true);
            updateSpeed(playerB, false, true);
        }
    }

    advancePos(playerA);
    advancePos(playerB);


    // collision
    collide(playerA, playerApx, playerApz, playerB, playerBpx, playerBpz, trackSize);

    simStepAdjustEnd(playerA, playerApz, track, trackSize);
    simStepAdjustEnd(playerB, playerBpz, track, trackSize);

    if (state == 1)
    {
        if (playerA.finished)
        {
            playerA.wins = 1;
            state = 2;
            stateTick = 0;
        }
        if (playerB.finished)
        {
            playerB.wins = 1;
            state = 2;
            stateTick = 0;
        }
        if (state == 2)
        {
            if (!playerA.wins)
                playerA.wins = 2;
            if (!playerB.wins)
                playerB.wins = 2;
        }
    }


    snd_motorFreq(0, speed2fnum(playerA.speed));
    snd_motorFreq(1, speed2fnum(playerB.speed));


#ifdef USESDL
    /*printf("lap=%d x=%f z=%f speed=%f kmh ang=%d | lap=%d x=%f z=%f speed=%f kmh ang=%d\n", playerA.lap, playerA.x, playerA.z, playerA.speed * 3.6, playerA.angle, playerB.lap, playerB.x, playerB.z, playerB.speed * 3.6, playerB.angle);
    fflush(stdout);*/
#endif

    if (stateTick < 1000)
        ++stateTick;
}
