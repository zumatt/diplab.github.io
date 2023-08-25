/*
    -----------------------------------------------------------
                  CALCULATE RESISTANCE VALUE
    -----------------------------------------------------------
*/

int ciprofloxacin[3] = {10, 12, 15};                        //Resistance values for ciprofloxacin
int doxycycline[3] = {20, 25, 30};                          //Resistance values for doxycycline
int gatifloxacin[3] = {9, 14, 16};                          //Resistance values for gatifloxacin
int levofloxacin[3] = {11, 15, 18};                         //Resistance values for levofloxacin
int minocycine[3] = {21, 15, 28};                           //Resistance values for minocycine
int tetracycline[3] = {16, 24, 31};                         //Resistance values for tetracycline

void checkResistance(){
  Serial.println("Data reading:");
  Serial.print("  - Bacteria: ");
  Serial.println(j_bacteria);
  Serial.print("  - Antibiotic 1: ");
  Serial.println(j_ab1);
  Serial.print("  - Antibiotic 2: ");
  Serial.println(j_ab2);
  Serial.print("  - Antibiotic 3: ");
  Serial.println(j_ab3);

  if(j_bacteria == "Acinetobacter"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[0];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[0];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[0];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[0];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[0];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[0];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[0];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[0];};
  } else if(j_bacteria == "Enterococcus"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[1];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[1];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[1];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[1];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[1];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[1];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[1];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[1];};
  } else if(j_bacteria == "Entrobacteriaceae"){
    if (j_ab1 == "Ciprofloxacin"){ ab1_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab1 == "Doxycycline"){ ab1_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab1 == "Gatifloxacin"){ ab1_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab1 == "Levofloxacin"){ ab1_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab1 == "Minocycine"){ ab1_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab1 == "Tetracycline"){ ab1_resistance = ab_resistance_multiplier * tetracycline[2];};

    if (j_ab2 == "Ciprofloxacin"){ ab2_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab2 == "Doxycycline"){ ab2_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab2 == "Gatifloxacin"){ ab2_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab2 == "Levofloxacin"){ ab2_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab2 == "Minocycine"){ ab2_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab2 == "Tetracycline"){ ab2_resistance = ab_resistance_multiplier * tetracycline[2];};
    
    if (j_ab3 == "Ciprofloxacin"){ ab3_resistance = ab_resistance_multiplier * ciprofloxacin[2];};
    if (j_ab3 == "Doxycycline"){ ab3_resistance = ab_resistance_multiplier * doxycycline[2];};
    if (j_ab3 == "Gatifloxacin"){ ab3_resistance = ab_resistance_multiplier * gatifloxacin[2];};
    if (j_ab3 == "Levofloxacin"){ ab3_resistance = ab_resistance_multiplier * levofloxacin[2];};
    if (j_ab3 == "Minocycine"){ ab3_resistance = ab_resistance_multiplier * minocycine[2];};
    if (j_ab3 == "Tetracycline"){ ab3_resistance = ab_resistance_multiplier * tetracycline[2];};
  }

  Serial.println("");
  Serial.println(" ---------------- ");
  Serial.println("");
  Serial.println("Data produced:");
  Serial.print("  - Resistance value 1: ");
  Serial.println(ab1_resistance);
  Serial.print("  - Resistance value 2: ");
  Serial.println(ab2_resistance);
  Serial.print("  - Resistance value 3: ");
  Serial.println(ab3_resistance);

}