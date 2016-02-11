<?php

/**
 * Implements hook_drush_command().
 */
function akamai_drush_command() {
  $items = array();

  $items['akamai-purge-url'] = array(
    'description' => 'Akamai purge URL.',
    'arguments' => array(
      'path' => 'A path to purge. You can provide as many paths you like. e.g. /node/1, /videos/earth',
    ),
    'options' => array(
      'action' => 'Purge action. invalidate or remove',
      'hostname' => 'Hostname. www.example.com',
      'network' => 'The Akamai network to use for cache purging. staging or production',
    ),
    'aliases' => array('ak-purge', 'akp'),
    'callback' => 'drush_akamai_purge_url',
  );

  return $items;
}

/**
 * Callback function for akamai-purge-url command.
 *
 * Runs akamai_purge_paths().
 */
function drush_akamai_purge_url() {
  $paths = func_get_args();
  $overrides = array();
  if (drush_get_option('action')) {
    $overrides['action'] = drush_get_option('action');
  }
  if (drush_get_option('hostname')) {
    $overrides['hostname'] = drush_get_option('hostname');
  }
  if (drush_get_option('network')) {
    $overrides['network'] = drush_get_option('network');
  }
  $result = akamai_purge_paths($paths);
  if ($result) {
    $message = t("Akamai cache purge request has been made successfully, please allow @estimated_seconds seconds for changes to take effect. The URLs purged were:\n", array('@estimated_seconds' => $result->estimatedSeconds)) . print_r($paths, TRUE);

    drush_print("$message\n");
  }
  else {
    // @todo Provide more detail about the problem.
    drush_set_error(t("There was a problem with your cache purge request."));
  }
}
