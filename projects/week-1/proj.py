teamsEncode = {
    0x0: "النصر",
    0x1: "الهلال",
    0x2: "الأهلي",
    0x3: "القادسية",
    0x4: "التعاون",
    0x5: "الاتحاد",
    0x6: "الإتفاق",
    0x7: "نيوم",
    0x8: "الحزم",
    0x9: "الخليج",
    0xA: "الفيحاء",
    0xB: "الشباب",
    0xC: "الفتح",
    0xD: "الخلود",
    0xE: "ضمك",
    0xF: "الرياض",
    0x10: "الأخدود",
    0x11: "النجمة",
}


matchInfoEncode = {
    "tournament": {
        0x0: "كأس الملك",
        0x1: "الدوري",
        0x2: "السوبر",
        0x3: "كأس ولي العهد",
    },
    "stadium": {0x0: "home", 0x1: "away"},
    "state": {0x0: "خسارة", 0x1: "تعادل", 0x2: "الغاء", 0x3: "فوز"},
    "refree": {0x0: "محلي", 0x1: "دولي"},
}


masks = {
    "team1": 0b0000_0000_0001_1111,
    "team2": 0b0000_0011_1110_0000,
    "matchState": 0b0000_1100_0000_0000,
    "stadium": 0b0001_0000_0000_0000,
    "tournament": 0b0110_0000_0000_0000,
    "refree": 0b1000_0000_0000_0000,
}


def unpack(inp):
    bTeam1 = inp & masks["team1"]
    bTeam2 = (inp & masks["team2"]) >> 5
    bMatchState = (inp & masks["matchState"]) >> 10
    bStadium = (inp & masks["stadium"]) >> 12
    bTournament = (inp & masks["tournament"]) >> 13
    bRefree = (inp & masks["refree"]) >> 15

    return bTeam1, bTeam2, bMatchState, bStadium, bTournament, bRefree


def decode(bTeam1, bTeam2, bMatchState, bStadium, bTournament, bRefree):
    if bTeam1 > len(teamsEncode) or bTeam2 > len(teamsEncode):
        print("there's only 17 teams ")
        exit(0)

    team1 = teamsEncode[bTeam1]
    team2 = teamsEncode[bTeam2]
    isHome = team1 if matchInfoEncode["stadium"][bStadium] == "home" else team2
    return (
        team1,
        team2,
        isHome,
        matchInfoEncode["state"][bMatchState],
        matchInfoEncode["tournament"][bTournament],
        matchInfoEncode["refree"][bRefree],
    )


def main():
    looper = True
    inp = 0b0
    while looper:
        inp = int(input("enter the input on Hex (base-16) "), 16)
        if inp.bit_length() <= 16:
            break
        print("must be 16 bit at most")

    bTeam1, bTeam2, bMatchState, bStadium, bTournament, bRefree = unpack(inp)
    team1, team2, isHome, matchState, tournament, refree = decode(
        bTeam1, bTeam2, bMatchState, bStadium, bTournament, bRefree
    )
    printInfo(team1, team2, isHome, matchState, tournament, refree)


def printInfo(team1, team2, stadium, matchState, tournament, refree):

    print(f"الفريق الأول: {team1}")
    print(f"الفريق الثاني:  {team2}")
    print(f"حالة المباراة: {matchState}")
    print(f"الملعب: {stadium}")
    print(f"البطولة: {tournament}")
    print(f"الحكم: {refree}")


main()
