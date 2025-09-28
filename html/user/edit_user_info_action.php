<?php
// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

require_once("../inc/boinc_db.inc");
require_once("../inc/user.inc");
require_once("../inc/util.inc");
require_once("../inc/user_util.inc");
require_once("../inc/countries.inc");

check_get_args(array("tnow", "ttok"));

$user = get_logged_in_user();
check_tokens($user->authenticator);

$name = post_str("user_name");
if (!is_valid_user_name($name, $reason)) {
    error_page($reason);
}
if (strlen($name) == 0) {
    error_page(tra("You must supply a name for your account."));
}
$name = BoincDb::escape_string($name);

$u = BoincUser::lookup(sprintf("name='%s'", $name));
if ($u && ($u->id != $user->id)) {
    error_page('That name is in use - go back and try another.');
}

$url = "";
$country = "";
$postal_code = "";
if (USER_URL) {
    $url = post_str("url", true);
    $x = sanitize_user_url($url);
    if ($x != $url) {
        error_page("Invalid URL");
    }
    $url = BoincDb::escape_string($x);
}
if (USER_COUNTRY) {
    $country = post_str("country");
    if (!is_valid_country($country)) {
        error_page("bad country");
    }
    $country = BoincDb::escape_string($country);
}
if (POSTAL_CODE) {
    $postal_code = BoincDb::escape_string(sanitize_tags(post_str("postal_code", true)));
}

$latitude_str = post_str("latitude", true);
if (strlen($latitude_str) && !is_numeric($latitude_str)) {
    error_page("Latitude must be a number.");
}
$latitude = (float) $latitude_str;
if ($latitude < -90 || $latitude > 90) {
    error_page("Latitude must be between -90 and 90.");
}

$longitude_str = post_str("longitude", true);
if (strlen($longitude_str) && !is_numeric($longitude_str)) {
    error_page("Longitude must be a number.");
}
$longitude = (float) $longitude_str;
if ($longitude < -180 || $longitude > 180) {
    error_page("Longitude must be between -180 and 180.");
}

$sun_only = post_str("sun_only", true) ? 1 : 0;

$result = $user->update(
    "name='$name', url='$url', country='$country', postal_code='$postal_code', latitude=$latitude, longitude=$longitude, sun_only=$sun_only"
);
if ($result) {
    Header("Location: ".HOME_PAGE);
} else {
    error_page(tra("Couldn't update user info."));
}

?>
