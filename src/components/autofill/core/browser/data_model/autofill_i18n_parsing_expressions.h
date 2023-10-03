// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file was generated by go/autofill-i18n-model-git. Do not
// manually edit.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_DATA_MODEL_AUTOFILL_I18N_PARSING_EXPRESSIONS_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_DATA_MODEL_AUTOFILL_I18N_PARSING_EXPRESSIONS_H_

#include "base/containers/fixed_flat_map.h"
#include "base/containers/flat_map.h"
#include "base/containers/span.h"
#include "base/memory/raw_ptr.h"
#include "components/autofill/core/browser/data_model/autofill_i18n_parsing_expression_components.h"
#include "components/autofill/core/browser/field_types.h"
#include "third_party/re2/src/re2/re2.h"

namespace autofill::i18n_model_definition {
namespace {

using CountryAndFieldType = std::pair<std::string_view, ServerFieldType>;

// Section for regular expressions.
constexpr char kRegularExpression_1[] = "(?:^|\\s+)"; // nocheck
constexpr char kRegularExpression_2[] = "(?:^|[,\\s]+)"; // nocheck
constexpr char kRegularExpression_3[] = "(?:남궁|사공|서문|선우|제갈|황보|독고|망절|欧阳|令狐|皇甫|上官|司徒|诸葛|司马|宇文|呼延|端木|張簡|歐陽|諸葛|申屠|尉遲|司馬|軒轅|夏侯)"; // nocheck
constexpr char kRegularExpression_4[] = "\\p{Hangul}"; // nocheck
constexpr char kRegularExpression_5[] = "\\p{Hangul}+"; // nocheck
constexpr char kRegularExpression_6[] = "(?:・|·|　|\\s+)"; // nocheck
constexpr char kRegularExpression_7[] = "(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st)"; // nocheck
constexpr char kRegularExpression_8[] = "(?:(?:b\\.a|ba|d\\.d\\.s|dds|ii|iii|iv|ix|jr|m\\.a|m\\.d|md|ms|ph\\.?d|sr|v|vi|vii|viii|x)\\.?)"; // nocheck
constexpr char kRegularExpression_9[] = "(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})"; // nocheck
constexpr char kRegularExpression_10[] = "(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+)"; // nocheck
constexpr char kRegularExpression_11[] = "(?:강전|남궁|독고|동방|망절|사공|서문|선우|소봉|어금|장곡|제갈|황목|황보)"; // nocheck
constexpr char kRegularExpression_12[] = "(?:Aguilar|Alonso|Álvarez|Amador|Betancourt|Blanco|Burgos|Castillo|Castro|Chávez|Colón|Contreras|Cortez|Cruz|Delgado|Diaz|Díaz|Domínguez|Estrada|Fernandez|Fernández|Flores|Fuentes|Garcia|García|Garza|Gil|Gómez|González|Guerrero|Gutiérrez|Guzmán|Hernández|Herrera|Iglesias|Jiménez|Juárez|Lopez|López|Luna|Marín|Marroquín|Martín|Martinez|Martínez|Medina|Méndez|Mendoza|Molina|Morales|Moreno|Muñoz|Narvaez|Navarro|Núñez|Ortega|Ortiz|Ortíz|Peña|Perez|Pérez|Ramírez|Ramos|Reyes|Rivera|Rodriguez|Rodríguez|Rojas|Romero|Rosario|Rubio|Ruiz|Ruíz|Salazar|Sanchez|Sánchez|Santana|Santiago|Santos|Sanz|Serrano|Soto|Suárez|Toro|Torres|Vargas|Vasquez|Vásquez|Vázquez|Velásquez)"; // nocheck
constexpr char kRegularExpression_13[] = "(?:(?:(?:Aguilar|Alonso|Álvarez|Amador|Betancourt|Blanco|Burgos|Castillo|Castro|Chávez|Colón|Contreras|Cortez|Cruz|Delgado|Diaz|Díaz|Domínguez|Estrada|Fernandez|Fernández|Flores|Fuentes|Garcia|García|Garza|Gil|Gómez|González|Guerrero|Gutiérrez|Guzmán|Hernández|Herrera|Iglesias|Jiménez|Juárez|Lopez|López|Luna|Marín|Marroquín|Martín|Martinez|Martínez|Medina|Méndez|Mendoza|Molina|Morales|Moreno|Muñoz|Narvaez|Navarro|Núñez|Ortega|Ortiz|Ortíz|Peña|Perez|Pérez|Ramírez|Ramos|Reyes|Rivera|Rodriguez|Rodríguez|Rojas|Romero|Rosario|Rubio|Ruiz|Ruíz|Salazar|Sanchez|Sánchez|Santana|Santiago|Santos|Sanz|Serrano|Soto|Suárez|Toro|Torres|Vargas|Vasquez|Vásquez|Vázquez|Velásquez)|\\s(?:y|e|i)\\s))"; // nocheck
constexpr char kRegularExpression_14[] = "(?:^(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+)(?:(?:・|·|　|\\s+)(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+))?$)"; // nocheck
constexpr char kRegularExpression_15[] = "(?:[^\\s,]+)"; // nocheck
constexpr char kRegularExpression_16[] = "(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)"; // nocheck
constexpr char kRegularExpression_17[] = "(?:[^,\\r\\n]+)"; // nocheck
constexpr char kRegularExpression_18[] = "\\s(?:y|e|i)\\s"; // nocheck
constexpr char kRegularExpression_19[] = "(?:y|e|i)"; // nocheck
constexpr char kRegularExpression_20[] = "(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?"; // nocheck
constexpr char kRegularExpression_21[] = "^(?:[A-Z]\\.?(?:(?:\\s|-)?[A-Z]\\.?)*)$"; // nocheck
constexpr char kRegularExpression_22[] = "(?:KM\\s+)?\\d+\\w?"; // nocheck
constexpr char kRegularExpression_23[] = "(?:apto\\.?|apt\\.?|apartamento|sala nº|sala|conjunto)"; // nocheck
constexpr char kRegularExpression_24[] = "(?:\\d+\\w?\\b|\\w\\b)"; // nocheck
constexpr char kRegularExpression_25[] = "andar"; // nocheck
constexpr char kRegularExpression_26[] = "(?P<NAME_FULL>(?P<NAME_LAST>(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+))(?:(?:・|·|　|\\s+)(?P<NAME_FIRST>(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+))))"; // nocheck
constexpr char kRegularExpression_27[] = "(?P<NAME_FULL>(?P<NAME_LAST>(?:남궁|사공|서문|선우|제갈|황보|독고|망절|欧阳|令狐|皇甫|上官|司徒|诸葛|司马|宇文|呼延|端木|張簡|歐陽|諸葛|申屠|尉遲|司馬|軒轅|夏侯))(?P<NAME_FIRST>(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+))?)"; // nocheck
constexpr char kRegularExpression_28[] = "(?P<NAME_FULL>(?P<NAME_LAST>(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo}))(?P<NAME_FIRST>(?:(?:\\p{Han}|\\p{Hangul}|\\p{Katakana}|\\p{Hiragana}|\\p{Bopomofo})+))?)"; // nocheck
constexpr char kRegularExpression_29[] = "(?P<NAME_FULL>(?P<NAME_LAST>(?:강전|남궁|독고|동방|망절|사공|서문|선우|소봉|어금|장곡|제갈|황목|황보))(?P<NAME_FIRST>\\p{Hangul}\\p{Hangul}+))"; // nocheck
constexpr char kRegularExpression_30[] = "(?P<NAME_FULL>(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))(?:(?:^|\\s+)(?:(?:b\\.a|ba|d\\.d\\.s|dds|ii|iii|iv|ix|jr|m\\.a|m\\.d|md|ms|ph\\.?d|sr|v|vi|vii|viii|x)\\.?))?)"; // nocheck
constexpr char kRegularExpression_31[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_FIRST>(?:[^\\s,]+)))?(?:(?:^|\\s+)(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))??(?:(?:^|\\s+)(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)))(?:(?:^|\\s+)(?:(?:b\\.a|ba|d\\.d\\.s|dds|ii|iii|iv|ix|jr|m\\.a|m\\.d|md|ms|ph\\.?d|sr|v|vi|vii|viii|x)\\.?))?)"; // nocheck
constexpr char kRegularExpression_32[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)))\\s*,\\s*(?P<NAME_FIRST>(?:[^\\s,]+))?(?:(?:^|\\s+)(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))??)"; // nocheck
constexpr char kRegularExpression_33[] = "(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)(?:(?:^|\\s+)(?:y|e|i))?(?:^|\\s+)(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))"; // nocheck
constexpr char kRegularExpression_34[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_FIRST>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))??(?:(?:^|\\s+)(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)(?:(?:^|\\s+)(?:y|e|i))?(?:^|\\s+)(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))))"; // nocheck
constexpr char kRegularExpression_35[] = "(?P<NAME_LAST>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))"; // nocheck
constexpr char kRegularExpression_36[] = "\\A\\s*"; // nocheck
constexpr char kRegularExpression_37[] = "(?P<ADDRESS_HOME_STREET_LOCATION>\\A\\s*(?P<ADDRESS_HOME_STREET_NAME>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))(?:(?:,\\s*|\\s+-\\s+|\\s+)(?i:nº\\s+)?(?P<ADDRESS_HOME_HOUSE_NUMBER>(?:KM\\s+)?\\d+\\w?)))"; // nocheck
constexpr char kRegularExpression_38[] = "(?P<ADDRESS_HOME_APT_NUM>(?P<ADDRESS_HOME_APT_NUM_type>(?:apto\\.?|apt\\.?|apartamento|sala nº|sala|conjunto))(?:(?:^|\\s+)(?P<ADDRESS_HOME_APT_NUM_name>(?:\\d+\\w?\\b|\\w\\b))))"; // nocheck
constexpr char kRegularExpression_39[] = "(?:andar\\s*(?P<ADDRESS_HOME_FLOOR>\\d+))"; // nocheck
constexpr char kRegularExpression_40[] = "(?:(?:[,-]\\s*|^)(?P<ADDRESS_HOME_FLOOR>\\d+)\\s*(?:º\\s*)?andar(?:\\s*[,-]|$))"; // nocheck
constexpr char kRegularExpression_41[] = "(?:(?:ponto de )?refer[êe]ncia(?::\\s*|\\s+)(?P<ADDRESS_HOME_LANDMARK>[^,\\n]+))"; // nocheck
constexpr char kRegularExpression_42[] = "(?P<ADDRESS_HOME_APT_NUM>(?P<ADDRESS_HOME_APT_NUM_type>(?:apto\\.?|apt\\.?|apartamento|sala nº|sala|conjunto))?(?:(?:^|\\s+)(?P<ADDRESS_HOME_APT_NUM_name>(?:\\d+\\w?\\b|\\w\\b))))"; // nocheck
constexpr char kRegularExpression_43[] = "(?:(?:S/Num\\.?|S/N|Sin Nombre)|(?:KM\\s+)?\\d+\\w?)"; // nocheck
constexpr char kRegularExpression_44[] = "(?:despacho|loc\\.?|local|int(?:erior|\\.?)|n[uú]m(?:ero|\\.)? int(?:erno|\\.)?|Apartamento|Apto\\.?|Departamento|apto\\.?)"; // nocheck
constexpr char kRegularExpression_45[] = "piso"; // nocheck
constexpr char kRegularExpression_46[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_FIRST>(?:[^\\s,]+)))?(?:(?:^|\\s+)(?P<NAME_MIDDLE>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))??(?:(?:^|\\s+)(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)))(?:(?:^|\\s+)(?:(?:b\\.a|ba|d\\.d\\.s|dds|ii|iii|iv|ix|jr|m\\.a|m\\.d|md|ms|ph\\.?d|sr|v|vi|vii|viii|x)\\.?))?)"; // nocheck
constexpr char kRegularExpression_47[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_LAST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+)))\\s*,\\s*(?P<NAME_FIRST>(?:[^\\s,]+))?(?:(?:^|\\s+)(?P<NAME_MIDDLE>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))??)"; // nocheck
constexpr char kRegularExpression_48[] = "(?P<NAME_LAST>(?P<NAME_LAST_FIRST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))(?:(?:^|\\s+)(?P<NAME_LAST_CONJUNCTION>(?:y|e|i)))?(?:(?:^|\\s+)(?P<NAME_LAST_SECOND>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))))"; // nocheck
constexpr char kRegularExpression_49[] = "(?P<NAME_FULL>(?:(?:Master|Mr\\.?|Miss\\.?|Mrs\\.?|Missus|Ms\\.?|Mx\\.?|M\\.?|Ma'am|Sir|Gentleman|Sire|Mistress|Madam|Ma'am|Dame|Lord|Lady|Esq|Excellency|Excellence|Her Honour|His Honour|Hon\\.?|The Right Honourable|The Most Honourable|Dr\\.?|PhD|DPhil|MD|DO|Prof\\.|Professor|QC|CL|Chancellor|Vice-Chancellor|Principle|Principal|President|Master|Warden|Dean|Regent|Rector|Provost|Director|Chief Executive|Imām|Shaykh|Muftī|Hāfiz|Hāfizah|Qārī|Mawlānā|Hājī|Sayyid|Sayyidah|Sharif|Eminent|Venerable|His Holiness|His Holiness|His All Holiness|His Beatitude|The Most Blessed|His Excellency|His Most Eminent Highness|His Eminence|Most Reverend Eminence|The Most Reverend|His Grace|His Lordship|The Reverend|Fr|Pr|Br|Sr|Elder|Rabbi|The Reverend|Cantor|Chief Rabbi|Grand Rabbi|Rebbetzin|Herr|Frau|Fräulein|Dame|PD|Doktor|Magister|Ingenieur|1lt|1st|2lt|2nd|3rd|admiral|capt|captain|col|cpt|dr|gen|general|lcdr|lt|ltc|ltg|ltjg|maj|major|mg|pastor|prof|rep|reverend|rev|sen|st))?(?:(?:^|\\s+)(?P<NAME_FIRST>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))??(?:(?:^|\\s+)(?P<NAME_LAST>(?P<NAME_LAST_FIRST>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))(?:(?:^|\\s+)(?P<NAME_LAST_CONJUNCTION>(?:y|e|i)))?(?:(?:^|\\s+)(?P<NAME_LAST_SECOND>(?:(?:a|ab|af|av|ap|abu|aït|al|ālam|aust|austre|bar|bath|bat|ben|bin|ibn|bet|bint|binti|binte|da|das|de|degli|dele|del|du|della|der|di|dos|du|e|el|fetch|vetch|fitz|i??|kil|gil|de le|de la|la|le|lille|lu|m|mac|mc|mck|mhic|mic|mala|mellom|myljom|na|ned|nedre|neder|nic|ni|nin|nord|norr|ny|o|ua|ui|opp|upp|öfver|ost|öst|öster|øst|øst|østre|över|øvste|øvre|øver|öz|pour|putra|putri|setia|tor|söder|sør|sønder|sør|syd|søndre|syndre|søre|ter|ter|tre|van|van der|väst|väster|verch|erch|vest|vestre|vesle|vetle|von|zu|von und zu)\\s)?(?:[^\\s,]+))))))"; // nocheck
constexpr char kRegularExpression_50[] = "(?P<NAME_LAST>(?P<NAME_LAST_SECOND>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))"; // nocheck
constexpr char kRegularExpression_51[] = "(?P<ADDRESS_HOME_STREET_LOCATION>\\A\\s*(?P<ADDRESS_HOME_STREET_NAME>(?:calle\\s+\\d+\\s+[^\\d,\\n\\r]*?|(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))(?:[,]?\\s+?(?:(?:#|No\\.?|número\\s)\\s*)?(?P<ADDRESS_HOME_HOUSE_NUMBER>(?:(?:S/Num\\.?|S/N|Sin Nombre)|(?:KM\\s+)?\\d+\\w?))))"; // nocheck
constexpr char kRegularExpression_52[] = "(?P<ADDRESS_HOME_APT_NUM>(?P<ADDRESS_HOME_APT_NUM_type>(?:despacho|loc\\.?|local|int(?:erior|\\.?)|n[uú]m(?:ero|\\.)? int(?:erno|\\.)?|Apartamento|Apto\\.?|Departamento|apto\\.?))(?:(?:^|\\s+)(?P<ADDRESS_HOME_APT_NUM_name>(?:\\d+\\w?\\b|\\w\\b))\\b))"; // nocheck
constexpr char kRegularExpression_53[] = "(?:piso\\s*(?P<ADDRESS_HOME_FLOOR>\\d+))"; // nocheck
constexpr char kRegularExpression_54[] = "(?:(?:Cerca del)(?P<ADDRESS_HOME_LANDMARK>[^,\\n]+))"; // nocheck
constexpr char kRegularExpression_55[] = "(?:\\b(?:x|Entre( Calles)?)\\s+(?P<ADDRESS_HOME_BETWEEN_STREETS>(?P<ADDRESS_HOME_BETWEEN_STREETS_1>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))(?:\\s+y\\s+)(?P<ADDRESS_HOME_BETWEEN_STREETS_2>(?:[^,\\r\\n]+))))"; // nocheck
constexpr char kRegularExpression_56[] = "(?P<ADDRESS_HOME_APT_NUM>(?P<ADDRESS_HOME_APT_NUM_type>(?:despacho|loc\\.?|local|int(?:erior|\\.?)|n[uú]m(?:ero|\\.)? int(?:erno|\\.)?|Apartamento|Apto\\.?|Departamento|apto\\.?))?(?:(?:^|\\s+)(?P<ADDRESS_HOME_APT_NUM_name>(?:\\d+\\w?\\b|\\w\\b))))"; // nocheck
constexpr char kRegularExpression_57[] = "(?:(°|º|\\.|\\s|-)*(floor|flur|fl|og|obergeschoss|ug|untergeschoss|geschoss|andar|piso|º)(\\.|\\s|-)*)"; // nocheck
constexpr char kRegularExpression_58[] = "(?:(?:og|obergeschoss|ug|untergeschoss)?\\d{0,3}\\w?(?:\\.? (?:og|obergeschoss|ug|untergeschoss))?|(?:og|eg|ug))"; // nocheck
constexpr char kRegularExpression_59[] = "(?:(apt|apartment|wohnung|apto|-)(\\.|\\s|-)*)"; // nocheck
constexpr char kRegularExpression_60[] = "(?:(\\.|\\s|-)*(ª))"; // nocheck
constexpr char kRegularExpression_61[] = "(?:((no|°|º|number)(\\.|-|\\s)*)?)"; // nocheck
constexpr char kRegularExpression_62[] = "(?:(?:((no|°|º|number)(\\.|-|\\s)*)?)(?P<ADDRESS_HOME_HOUSE_NUMBER>(?:\\d+\\w?))(th\\.|\\.)?)(?:(?:^|\\s+)(?P<ADDRESS_HOME_STREET_NAME>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))"; // nocheck
constexpr char kRegularExpression_63[] = "(?P<ADDRESS_HOME_STREET_NAME>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))(?:(?:^|[,\\s]+)(?:((no|°|º|number)(\\.|-|\\s)*)?)(?P<ADDRESS_HOME_HOUSE_NUMBER>(?:\\d+\\w?))(th\\.|\\.)?)"; // nocheck
constexpr char kRegularExpression_64[] = "(?:(?:(apt|apartment|wohnung|apto|-)(\\.|\\s|-)*)(?P<ADDRESS_HOME_APT_NUM>(?:(\\d{0,3}\\w?))))"; // nocheck
constexpr char kRegularExpression_65[] = "(?:(-\\s*)?(?P<ADDRESS_HOME_APT_NUM>(?:(\\d{1,3}\\w?|\\w)))(?:(\\.|\\s|-)*(ª)))?"; // nocheck
constexpr char kRegularExpression_66[] = "(?:(?:(°|º|\\.|\\s|-)*(ADDRESS_HOME_FLOOR|flur|fl|og|obergeschoss|ug|untergeschoss|geschoss|andar|piso|º)(\\.|\\s|-)*)(?P<ADDRESS_HOME_FLOOR>(?:(\\d{0,3}\\w?))))"; // nocheck
constexpr char kRegularExpression_67[] = "(?:(?P<ADDRESS_HOME_FLOOR>(?:(\\d{1,3}\\w?|\\w)))(?:(°|º|\\.|\\s|-)*(ADDRESS_HOME_FLOOR|flur|fl|og|obergeschoss|ug|untergeschoss|geschoss|andar|piso|º)(\\.|\\s|-)*))"; // nocheck
constexpr char kRegularExpression_68[] = "(?P<ADDRESS_HOME_STREET_ADDRESS>(?:(?:(?:((no|°|º|number)(\\.|-|\\s)*)?)(?P<ADDRESS_HOME_HOUSE_NUMBER>(?:\\d+\\w?))(th\\.|\\.)?)(?:(?:^|\\s+)(?P<ADDRESS_HOME_STREET_NAME>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?)))|(?P<ADDRESS_HOME_STREET_NAME__2>(?:[^\\s,]+(?:[^\\S\\r\\n]+[^\\s,]+)*?))(?:(?:^|[,\\s]+)(?:((no|°|º|number)(\\.|-|\\s)*)?)(?P<ADDRESS_HOME_HOUSE_NUMBER__2>(?:\\d+\\w?))(th\\.|\\.)?))(?:(?:^|[,\\s]+)(?P<ADDRESS_HOME_SUBPREMISE>(?:(?:(?:(?:(°|º|\\.|\\s|-)*(ADDRESS_HOME_FLOOR|flur|fl|og|obergeschoss|ug|untergeschoss|geschoss|andar|piso|º)(\\.|\\s|-)*)(?P<ADDRESS_HOME_FLOOR>(?:(\\d{0,3}\\w?))))|(?:(?P<ADDRESS_HOME_FLOOR__2>(?:(\\d{1,3}\\w?|\\w)))(?:(°|º|\\.|\\s|-)*(ADDRESS_HOME_FLOOR|flur|fl|og|obergeschoss|ug|untergeschoss|geschoss|andar|piso|º)(\\.|\\s|-)*)))(?:(?:^|[,\\s]+)(?:(?:(?:(apt|apartment|wohnung|apto|-)(\\.|\\s|-)*)(?P<ADDRESS_HOME_APT_NUM>(?:(\\d{0,3}\\w?))))|(?:(-\\s*)?(?P<ADDRESS_HOME_APT_NUM__2>(?:(\\d{1,3}\\w?|\\w)))(?:(\\.|\\s|-)*(ª)))?))?|(?:(?:(?:(apt|apartment|wohnung|apto|-)(\\.|\\s|-)*)(?P<ADDRESS_HOME_APT_NUM__3>(?:(\\d{0,3}\\w?))))|(?:(-\\s*)?(?P<ADDRESS_HOME_APT_NUM__4>(?:(\\d{1,3}\\w?|\\w)))(?:(\\.|\\s|-)*(ª)))?))))?)"; // nocheck


// Section for singular decomposition(s).
constexpr Decomposition kDecompositionList[] = {
    Decomposition(kRegularExpression_26, true, true),
    Decomposition(kRegularExpression_29, true, true),
    Decomposition(kRegularExpression_27, true, true),
    Decomposition(kRegularExpression_28, true, true),
    Decomposition(kRegularExpression_34, true, true),
    Decomposition(kRegularExpression_30, true, true),
    Decomposition(kRegularExpression_32, true, true),
    Decomposition(kRegularExpression_31, true, true),
    Decomposition(kRegularExpression_37, true, true),
    Decomposition(kRegularExpression_42, true, true),
    Decomposition(kRegularExpression_49, true, true),
    Decomposition(kRegularExpression_47, true, true),
    Decomposition(kRegularExpression_46, true, true),
    Decomposition(kRegularExpression_51, true, true),
    Decomposition(kRegularExpression_55, true, true),
    Decomposition(kRegularExpression_56, true, true),
    Decomposition(kRegularExpression_68, true, true),
};

// Section for singular extract part(s).
constexpr ExtractPart kExtractPartList[]{
    ExtractPart("", kRegularExpression_38),
    ExtractPart("", kRegularExpression_39),
    ExtractPart("", kRegularExpression_40),
    ExtractPart("", kRegularExpression_41),
    ExtractPart("", kRegularExpression_37),
    ExtractPart("", kRegularExpression_52),
    ExtractPart("", kRegularExpression_53),
    ExtractPart("", kRegularExpression_54),
    ExtractPart("", kRegularExpression_55),
    ExtractPart("", kRegularExpression_51),
};

// Section for decomposition cascades and their alternatives.
constexpr AutofillParsingProcess const* kDecompositionCascade_0_Alternatives[]{ &kDecompositionList[0], &kDecompositionList[1], &kDecompositionList[2], &kDecompositionList[3]};
constexpr DecompositionCascade kDecompositionCascade_0 = DecompositionCascade(kRegularExpression_14, kDecompositionCascade_0_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_1_Alternatives[]{ &kDecompositionList[4]};
constexpr DecompositionCascade kDecompositionCascade_1 = DecompositionCascade(kRegularExpression_13, kDecompositionCascade_1_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_2_Alternatives[]{ &kDecompositionList[5], &kDecompositionList[6], &kDecompositionList[7]};
constexpr DecompositionCascade kDecompositionCascade_2 = DecompositionCascade("", kDecompositionCascade_2_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_3_Alternatives[]{ &kDecompositionCascade_0, &kDecompositionCascade_1, &kDecompositionCascade_2};
constexpr DecompositionCascade kDecompositionCascade_3 = DecompositionCascade("", kDecompositionCascade_3_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_4_Alternatives[]{ &kDecompositionList[0], &kDecompositionList[1], &kDecompositionList[2], &kDecompositionList[3]};
constexpr DecompositionCascade kDecompositionCascade_4 = DecompositionCascade(kRegularExpression_14, kDecompositionCascade_4_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_5_Alternatives[]{ &kDecompositionList[10]};
constexpr DecompositionCascade kDecompositionCascade_5 = DecompositionCascade(kRegularExpression_13, kDecompositionCascade_5_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_6_Alternatives[]{ &kDecompositionList[5], &kDecompositionList[11], &kDecompositionList[12]};
constexpr DecompositionCascade kDecompositionCascade_6 = DecompositionCascade("", kDecompositionCascade_6_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_7_Alternatives[]{ &kDecompositionCascade_4, &kDecompositionCascade_5, &kDecompositionCascade_6};
constexpr DecompositionCascade kDecompositionCascade_7 = DecompositionCascade("", kDecompositionCascade_7_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_8_Alternatives[]{ &kDecompositionList[0], &kDecompositionList[1], &kDecompositionList[2], &kDecompositionList[3]};
constexpr DecompositionCascade kDecompositionCascade_8 = DecompositionCascade(kRegularExpression_14, kDecompositionCascade_8_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_9_Alternatives[]{ &kDecompositionList[10]};
constexpr DecompositionCascade kDecompositionCascade_9 = DecompositionCascade(kRegularExpression_13, kDecompositionCascade_9_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_10_Alternatives[]{ &kDecompositionList[5], &kDecompositionList[6], &kDecompositionList[7]};
constexpr DecompositionCascade kDecompositionCascade_10 = DecompositionCascade("", kDecompositionCascade_10_Alternatives);
constexpr AutofillParsingProcess const* kDecompositionCascade_11_Alternatives[]{ &kDecompositionCascade_8, &kDecompositionCascade_9, &kDecompositionCascade_10};
constexpr DecompositionCascade kDecompositionCascade_11 = DecompositionCascade("", kDecompositionCascade_11_Alternatives);

// Section for extract parts and their pieces.
constexpr ExtractPart const* kExtractParts_0_Pieces[]{&kExtractPartList[0],&kExtractPartList[1],&kExtractPartList[2]};
constexpr ExtractParts kExtractParts_0 = ExtractParts("", kExtractParts_0_Pieces);
constexpr ExtractPart const* kExtractParts_1_Pieces[]{&kExtractPartList[0],&kExtractPartList[1],&kExtractPartList[2]};
constexpr ExtractParts kExtractParts_1 = ExtractParts("", kExtractParts_1_Pieces);
constexpr ExtractPart const* kExtractParts_2_Pieces[]{&kExtractPartList[0],&kExtractPartList[1],&kExtractPartList[2],&kExtractPartList[3]};
constexpr ExtractParts kExtractParts_2 = ExtractParts("", kExtractParts_2_Pieces);
constexpr ExtractPart const* kExtractParts_3_Pieces[]{&kExtractPartList[4],&kExtractPartList[0],&kExtractPartList[1],&kExtractPartList[2],&kExtractPartList[3]};
constexpr ExtractParts kExtractParts_3 = ExtractParts("", kExtractParts_3_Pieces);
constexpr ExtractPart const* kExtractParts_4_Pieces[]{&kExtractPartList[5],&kExtractPartList[6]};
constexpr ExtractParts kExtractParts_4 = ExtractParts("", kExtractParts_4_Pieces);
constexpr ExtractPart const* kExtractParts_5_Pieces[]{&kExtractPartList[7],&kExtractPartList[8]};
constexpr ExtractParts kExtractParts_5 = ExtractParts("", kExtractParts_5_Pieces);
constexpr ExtractPart const* kExtractParts_6_Pieces[]{&kExtractPartList[7],&kExtractPartList[8]};
constexpr ExtractParts kExtractParts_6 = ExtractParts("", kExtractParts_6_Pieces);
constexpr ExtractPart const* kExtractParts_7_Pieces[]{&kExtractPartList[9],&kExtractPartList[5],&kExtractPartList[6],&kExtractPartList[7],&kExtractPartList[8]};
constexpr ExtractParts kExtractParts_7 = ExtractParts("", kExtractParts_7_Pieces);
}  // namespace

// A lookup map for parsing expressions for countries and field types.
constexpr auto kAutofillParsingRulesMap =
    base::MakeFixedFlatMap<CountryAndFieldType, raw_ptr<const AutofillParsingProcess>>({
      {{"BR", NAME_FULL}, &kDecompositionCascade_3},
      {{"BR", ADDRESS_HOME_STREET_LOCATION}, &kDecompositionList[8]},
      {{"BR", ADDRESS_HOME_SUBPREMISE}, &kExtractParts_0},
      {{"BR", ADDRESS_HOME_OVERFLOW}, &kExtractParts_1},
      {{"BR", ADDRESS_HOME_OVERFLOW_AND_LANDMARK}, &kExtractParts_2},
      {{"BR", ADDRESS_HOME_STREET_ADDRESS}, &kExtractParts_3},
      {{"BR", ADDRESS_HOME_APT_NUM}, &kDecompositionList[9]},
      {{"MX", NAME_FULL}, &kDecompositionCascade_7},
      {{"MX", ADDRESS_HOME_STREET_LOCATION}, &kDecompositionList[13]},
      {{"MX", ADDRESS_HOME_SUBPREMISE}, &kExtractParts_4},
      {{"MX", ADDRESS_HOME_BETWEEN_STREETS}, &kDecompositionList[14]},
      {{"MX", ADDRESS_HOME_BETWEEN_STREETS_OR_LANDMARK}, &kExtractParts_5},
      {{"MX", ADDRESS_HOME_OVERFLOW}, &kExtractParts_6},
      {{"MX", ADDRESS_HOME_STREET_ADDRESS}, &kExtractParts_7},
      {{"MX", ADDRESS_HOME_APT_NUM}, &kDecompositionList[15]},
      {{"XX", NAME_FULL}, &kDecompositionCascade_11},
      {{"XX", ADDRESS_HOME_STREET_ADDRESS}, &kDecompositionList[16]}
      });

}  // namespace autofill::i18n_model_definition

#endif  // COMPONENTS_AUTOFILL_CORE_BROWSER_DATA_MODEL_AUTOFILL_I18N_PARSING_EXPRESSIONS_H_