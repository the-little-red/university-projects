require 'test_helper'

class AddressesUsersControllerTest < ActionController::TestCase
  setup do
    @addresses_user = addresses_users(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:addresses_users)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create addresses_user" do
    assert_difference('AddressesUser.count') do
      post :create, addresses_user: { address_id: @addresses_user.address_id, user_id: @addresses_user.user_id }
    end

    assert_redirected_to addresses_user_path(assigns(:addresses_user))
  end

  test "should show addresses_user" do
    get :show, id: @addresses_user
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @addresses_user
    assert_response :success
  end

  test "should update addresses_user" do
    patch :update, id: @addresses_user, addresses_user: { address_id: @addresses_user.address_id, user_id: @addresses_user.user_id }
    assert_redirected_to addresses_user_path(assigns(:addresses_user))
  end

  test "should destroy addresses_user" do
    assert_difference('AddressesUser.count', -1) do
      delete :destroy, id: @addresses_user
    end

    assert_redirected_to addresses_users_path
  end
end
