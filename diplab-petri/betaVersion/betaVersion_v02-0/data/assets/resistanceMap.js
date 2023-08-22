function resistanceMap(bact){
    if(bact == "COLI"){
        const colihMap = new Map([
            ["CMX30", 8],
            ["TE30", 2],
            ["CIP5", 5]
          ]);
        //To get a value: coliMap.get("CMX30");
    } else if(bact == "STAPH"){
        const staphMap = new Map([
            ["CMX30", 5],
            ["TE30", 2],
            ["CIP5", 7]
          ]);
        //To get a value: staphMap.get("CMX30");
    } else if(bact == "SLM"){
        const slmhMap = new Map([
            ["CMX30", 2],
            ["TE30", 8],
            ["CIP5", 4]
          ]);
        //To get a value: slmMap.get("CMX30");
    }
}